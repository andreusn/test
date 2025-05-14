#ifndef SERVERGUI_H
#define SERVERGUI_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>
#include "serveradmin.h"
#include "databasehandler.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ServerGUI; }
QT_END_NAMESPACE

class ServerGUI : public QMainWindow
{
    Q_OBJECT

public:
    ServerGUI(QWidget* parent = nullptr);
    ~ServerGUI();

private slots:
    void on_startServerButton_clicked();
    void on_stopServerButton_clicked();
    void on_banUserButton_clicked();
    void on_kickUserButton_clicked();
    void on_deleteMessageButton_clicked();
    void on_refreshButton_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_sendGlobalMessageButton_clicked();

    void updateUserList();
    void updateMessageList();
    void updateConnectedClients();
    void logMessage(const QString& message);

    void handleNewConnection(QTcpSocket* client);
    void handleClientDisconnected(QTcpSocket* client);
    void handleUserBanned(int userId);
    void handleUserKicked(int userId);
    void handleNewMessageReceived(const QString& sender, const QString& message, bool isPrivate);

private:
    Ui::ServerGUI* ui;
    ServerAdmin* serverAdmin;
    DatabaseHandler* dbHandler;
    QStandardItemModel* usersModel;
    QStandardItemModel* messagesModel;
    QStandardItemModel* clientsModel;

    void setupTables();
    void loadInitialData();
    void applyStyles();
};
#endif // SERVERGUI_H