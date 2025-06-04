#include "serveradmin.h"
#include <QDebug>
using namespace Qt;

ServerAdmin::ServerAdmin(QObject* parent, DatabaseHandler* dbHandler)
    : QObject(parent), dbHandler(dbHandler)
{
    tcpServer = new QTcpServer(this);
    connect(tcpServer, &QTcpServer::newConnection, this, &ServerAdmin::handleNewConnection);
}

ServerAdmin::~ServerAdmin()
{
    stopServer();
    delete tcpServer;
}

bool ServerAdmin::startServer(quint16 port)
{
    if (!tcpServer->listen(QHostAddress::Any, port)) {
        qDebug() << "Server could not start:" << tcpServer->errorString();
        return false;
    }
    qDebug() << "Server started on port" << port;
    return true;
}

void ServerAdmin::stopServer()
{
    if (tcpServer->isListening()) {
        tcpServer->close();
        qDebug() << "Server stopped";
    }
}

QVector<QTcpSocket*> ServerAdmin::getConnectedClients() const
{
    QVector<QTcpSocket*> sockets;
    for (auto it = clients.constBegin(); it != clients.constEnd(); ++it) {
        sockets.append(it.key());
    }
    return sockets;
}

QVector<User> ServerAdmin::getBannedUsers() const
{
    if (dbHandler) {
        auto users = dbHandler->getAllUsers();
        QVector<User> bannedUsers;
        for (const auto& user : users) {
            if (user.isBanned) {
                bannedUsers.append(user);
            }
        }
        return bannedUsers;
    }
    return QVector<User>();
}

void ServerAdmin::banUser(int userId)
{
    if (dbHandler && dbHandler->banUser(userId, true)) {
        emit userBanned(userId);

        for (auto it = clients.begin(); it != clients.end(); ++it) {
            if (it.value() == userId) {
                it.key()->disconnectFromHost();
                break;
            }
        }
    }
}

void ServerAdmin::kickUser(int userId)
{
    if (dbHandler) {
        dbHandler->disconnectUser(userId);
        emit userKicked(userId);

        for (auto it = clients.begin(); it != clients.end(); ++it) {
            if (it.value() == userId) {
                it.key()->disconnectFromHost();
                break;
            }
        }
    }
}

void ServerAdmin::sendServerMessage(const QString& message)
{
    for (auto socket : clients.keys()) {
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << QString("SERVER") << message << false;
        socket->write(data);
    }
}

void ServerAdmin::handleNewConnection()
{
    QTcpSocket* clientSocket = tcpServer->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &ServerAdmin::readClientData);
    connect(clientSocket, &QTcpSocket::disconnected, this, &ServerAdmin::clientDisconnected);

    qDebug() << "New connection from:" << clientSocket->peerAddress().toString();
    emit newConnection(clientSocket);
}

void ServerAdmin::readClientData()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    QDataStream in(clientSocket);
    in.setVersion(QDataStream::Qt_5_15);

    while (clientSocket->bytesAvailable() > 0) {
        QString sender, message;
        bool isPrivate;
        in >> sender >> message >> isPrivate;

        qDebug() << "Message from" << sender << ":" << message;
        emit newMessageReceived(sender, message, isPrivate);
    }
}

void ServerAdmin::clientDisconnected()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    int userId = clients.value(clientSocket, -1);
    if (userId != -1 && dbHandler) {
        dbHandler->disconnectUser(userId);
    }

    clients.remove(clientSocket);
    clientSocket->deleteLater();

    qDebug() << "Client disconnected";
    emit clientDisconnected(clientSocket);
}