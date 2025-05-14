#include "servergui.h"
#include "ui_servergui.h"
#include <QMessageBox>
#include <QDateTime>
#include <QStyledItemDelegate>
#include <QPainter>
using namespace Qt;

class AlternatingRowDelegate : public QStyledItemDelegate {
public:
    AlternatingRowDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);

        if (index.row() % 2 == 0) {
            opt.backgroundBrush = QBrush(QColor(240, 240, 240));
        }
        else {
            opt.backgroundBrush = QBrush(Qt::white);
        }

        QStyledItemDelegate::paint(painter, opt, index);
    }
};

ServerGUI::ServerGUI(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerGUI)
{
    ui->setupUi(this);

    dbHandler = new DatabaseHandler(this);
    if (!dbHandler->connectToDatabase("localhost", "messenger_db", "admin", "admin123")) {
        QMessageBox::critical(this, "Database Error", "Failed to connect to database!");
    }

    serverAdmin = new ServerAdmin(this, dbHandler);

    setupTables();
    loadInitialData();
    applyStyles();

    connect(serverAdmin, &ServerAdmin::newConnection, this, &ServerGUI::handleNewConnection);
    connect(serverAdmin, &ServerAdmin::clientDisconnected, this, &ServerGUI::handleClientDisconnected);
    connect(serverAdmin, &ServerAdmin::userBanned, this, &ServerGUI::handleUserBanned);
    connect(serverAdmin, &ServerAdmin::userKicked, this, &ServerGUI::handleUserKicked);
    connect(serverAdmin, &ServerAdmin::newMessageReceived, this, &ServerGUI::handleNewMessageReceived);

    setWindowTitle("Messenger Server Admin");
    resize(1000, 600);
}

ServerGUI::~ServerGUI()
{
    delete ui;
    serverAdmin->stopServer();
}

void ServerGUI::setupTables()
{
    usersModel = new QStandardItemModel(this);
    usersModel->setHorizontalHeaderLabels({ "ID", "Username", "Email", "Banned", "Online", "Last Seen" });
    ui->usersTableView->setModel(usersModel);
    ui->usersTableView->setItemDelegate(new AlternatingRowDelegate(this));
    ui->usersTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->usersTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->usersTableView->setSortingEnabled(true);

    messagesModel = new QStandardItemModel(this);
    messagesModel->setHorizontalHeaderLabels({ "ID", "Sender", "Receiver", "Content", "Time", "Private" });
    ui->messagesTableView->setModel(messagesModel);
    ui->messagesTableView->setItemDelegate(new AlternatingRowDelegate(this));
    ui->messagesTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->messagesTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->messagesTableView->setSortingEnabled(true);

    clientsModel = new QStandardItemModel(this);
    clientsModel->setHorizontalHeaderLabels({ "ID", "Username", "IP Address", "Connected" });
    ui->clientsTableView->setModel(clientsModel);
    ui->clientsTableView->setItemDelegate(new AlternatingRowDelegate(this));
    ui->clientsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->clientsTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->usersTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->messagesTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->clientsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
}

void ServerGUI::loadInitialData()
{
    updateUserList();
    updateMessageList();
}

void ServerGUI::applyStyles()
{
    setStyleSheet(R"(
        QMainWindow {
            background-color: #f5f5f5;
        }
        QTableView {
            border: 1px solid #ddd;
            background-color: white;
            alternate-background-color: #f9f9f9;
        }
        QHeaderView::section {
            background-color: #4CAF50;
            color: white;
            padding: 4px;
            border: 1px solid #ddd;
        }
        QPushButton {
            background-color: #4CAF50;
            color: white;
            border: none;
            padding: 5px 10px;
            border-radius: 3px;
        }
        QPushButton:hover {
            background-color: #45a049;
        }
        QPushButton:disabled {
            background-color: #cccccc;
        }
        QTabWidget::pane {
            border: 1px solid #ddd;
            background: white;
        }
        QTabBar::tab {
            background: #f1f1f1;
            padding: 8px;
            border: 1px solid #ddd;
            border-bottom: none;
            border-top-left-radius: 3px;
            border-top-right-radius: 3px;
        }
        QTabBar::tab:selected {
            background: white;
            border-bottom: 2px solid #4CAF50;
        }
        QLineEdit, QTextEdit {
            border: 1px solid #ddd;
            padding: 3px;
        }
        QStatusBar {
            background-color: #e0e0e0;
            color: #333;
        }
    )");

    ui->logTextEdit->setStyleSheet("background-color: white; border: 1px solid #ddd;");
}

void ServerGUI::updateUserList()
{
    if (!dbHandler) return;

    usersModel->clear();
    usersModel->setHorizontalHeaderLabels({ "ID", "Username", "Email", "Banned", "Online", "Last Seen" });

    auto users = dbHandler->getAllUsers();
    for (const auto& user : users) {
        QList<QStandardItem*> rowItems;
        rowItems << new QStandardItem(QString::number(user.id));
        rowItems << new QStandardItem(user.username);
        rowItems << new QStandardItem(user.email);

        QStandardItem* bannedItem = new QStandardItem();
        bannedItem->setCheckable(true);
        bannedItem->setCheckState(user.isBanned ? Qt::Checked : Qt::Unchecked);
        bannedItem->setText(user.isBanned ? "Yes" : "No");
        bannedItem->setEditable(false);
        rowItems << bannedItem;

        QStandardItem* onlineItem = new QStandardItem();
        onlineItem->setCheckable(true);
        onlineItem->setCheckState(user.isOnline ? Qt::Checked : Qt::Unchecked);
        onlineItem->setText(user.isOnline ? "Yes" : "No");
        onlineItem->setEditable(false);
        rowItems << onlineItem;

        rowItems << new QStandardItem(user.lastSeen.toString("yyyy-MM-dd HH:mm:ss"));

        usersModel->appendRow(rowItems);
    }
}

void ServerGUI::updateMessageList()
{
    if (!dbHandler) return;

    messagesModel->clear();
    messagesModel->setHorizontalHeaderLabels({ "ID", "Sender", "Receiver", "Content", "Time", "Private" });

    auto messages = dbHandler->getAllMessages();
    for (const auto& msg : messages) {
        QString senderName = QString::number(msg.senderId);
        QString receiverName = msg.isPrivate ? QString::number(msg.receiverId) : "All";

        QList<QStandardItem*> rowItems;
        rowItems << new QStandardItem(QString::number(msg.id));
        rowItems << new QStandardItem(senderName);
        rowItems << new QStandardItem(receiverName);
        rowItems << new QStandardItem(msg.content);
        rowItems << new QStandardItem(msg.timestamp.toString("yyyy-MM-dd HH:mm:ss"));

        QStandardItem* privateItem = new QStandardItem();
        privateItem->setCheckable(true);
        privateItem->setCheckState(msg.isPrivate ? Qt::Checked : Qt::Unchecked);
        privateItem->setText(msg.isPrivate ? "Yes" : "No");
        privateItem->setEditable(false);
        rowItems << privateItem;

        messagesModel->appendRow(rowItems);
    }
}

void ServerGUI::updateConnectedClients()
{
    clientsModel->clear();
    clientsModel->setHorizontalHeaderLabels({ "ID", "Username", "IP Address", "Connected" });

    if (!serverAdmin) return;

    auto clients = serverAdmin->getConnectedClients();
    for (auto socket : clients) {
        int userId = serverAdmin->getUserId(socket);
        auto user = dbHandler->getUserById(userId);

        QList<QStandardItem*> rowItems;
        rowItems << new QStandardItem(QString::number(userId));
        rowItems << new QStandardItem(user.username);
        rowItems << new QStandardItem(socket->peerAddress().toString());
        rowItems << new QStandardItem(socket->state() == QTcpSocket::ConnectedState ? "Yes" : "No");

        clientsModel->appendRow(rowItems);
    }
}

void ServerGUI::logMessage(const QString& message)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    ui->logTextEdit->appendPlainText(QString("[%1] %2").arg(timestamp, message));
}

void ServerGUI::on_startServerButton_clicked()
{
    int port = ui->portSpinBox->value();
    if (serverAdmin->startServer(port)) {
        ui->startServerButton->setEnabled(false);
        ui->stopServerButton->setEnabled(true);
        logMessage(QString("Server started on port %1").arg(port));
    }
}

void ServerGUI::on_stopServerButton_clicked()
{
    serverAdmin->stopServer();
    ui->startServerButton->setEnabled(true);
    ui->stopServerButton->setEnabled(false);
    logMessage("Server stopped");
}

void ServerGUI::on_banUserButton_clicked()
{
    QModelIndexList selected = ui->usersTableView->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select a user first!");
        return;
    }

    int userId = usersModel->item(selected.first().row(), 0)->text().toInt();
    if (QMessageBox::question(this, "Confirm Ban",
        QString("Are you sure you want to ban user ID %1?").arg(userId),
        QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        serverAdmin->banUser(userId);
        logMessage(QString("User ID %1 banned").arg(userId));
    }
}

void ServerGUI::on_kickUserButton_clicked()
{
    QModelIndexList selected = ui->usersTableView->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select a user first!");
        return;
    }

    int userId = usersModel->item(selected.first().row(), 0)->text().toInt();
    if (QMessageBox::question(this, "Confirm Kick",
        QString("Are you sure you want to kick user ID %1?").arg(userId),
        QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        serverAdmin->kickUser(userId);
        logMessage(QString("User ID %1 kicked").arg(userId));
    }
}

void ServerGUI::on_deleteMessageButton_clicked()
{
    QModelIndexList selected = ui->messagesTableView->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select a message first!");
        return;
    }

    int messageId = messagesModel->item(selected.first().row(), 0)->text().toInt();
    if (QMessageBox::question(this, "Confirm Delete",
        QString("Are you sure you want to delete message ID %1?").arg(messageId),
        QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        if (dbHandler->deleteMessage(messageId)) {
            updateMessageList();
            logMessage(QString("Message ID %1 deleted").arg(messageId));
        }
        else {
            QMessageBox::warning(this, "Error", "Failed to delete message!");
        }
    }
}

void ServerGUI::on_refreshButton_clicked()
{
    int currentTab = ui->tabWidget->currentIndex();
    if (currentTab == 0) {
        updateUserList();
    }
    else if (currentTab == 1) {
        updateMessageList();
    }
    else if (currentTab == 2) {
        updateConnectedClients();
    }
    logMessage("Data refreshed");
}

void ServerGUI::on_tabWidget_currentChanged(int index)
{
    if (index == 2) { 
        updateConnectedClients();
    }
}

void ServerGUI::on_sendGlobalMessageButton_clicked()
{
    QString message = ui->globalMessageTextEdit->toPlainText().trimmed();
    if (message.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Message cannot be empty!");
        return;
    }

    serverAdmin->sendServerMessage(message);
    ui->globalMessageTextEdit->clear();
    logMessage("Global message sent to all users");
}

void ServerGUI::handleNewConnection(QTcpSocket* client)
{
    logMessage(QString("New connection from %1").arg(client->peerAddress().toString()));
    updateConnectedClients();
}

void ServerGUI::handleClientDisconnected(QTcpSocket* client)
{
    logMessage(QString("Client %1 disconnected").arg(client->peerAddress().toString()));
    updateConnectedClients();
}

void ServerGUI::handleUserBanned(int userId)
{
    logMessage(QString("User ID %1 banned").arg(userId));
    updateUserList();
    updateConnectedClients();
}

void ServerGUI::handleUserKicked(int userId)
{
    logMessage(QString("User ID %1 kicked").arg(userId));
    updateUserList();
    updateConnectedClients();
}

void ServerGUI::handleNewMessageReceived(const QString& sender, const QString& message, bool isPrivate)
{
    logMessage(QString("New %1 message from %2: %3")
        .arg(isPrivate ? "private" : "public", sender, message));
    updateMessageList();
}