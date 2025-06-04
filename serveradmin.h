#ifndef SERVERADMIN_H
#define SERVERADMIN_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QVector>
#include <QString>
#include <QByteArray>
#include <QDataStream>
#include <QDebug>
#include "databasehandler.h"

class ServerAdmin : public QObject
{
    Q_OBJECT
public:
    explicit ServerAdmin(QObject* parent = nullptr, DatabaseHandler* dbHandler = nullptr);
    ~ServerAdmin();

    bool startServer(quint16 port);
    void stopServer();

    QVector<QTcpSocket*> getConnectedClients() const;
    QVector<User> getBannedUsers() const;

public slots:
    void banUser(int userId);
    void kickUser(int userId);
    void sendServerMessage(const QString& message);

signals:
    void newConnection(QTcpSocket* client);
    void clientDisconnected(QTcpSocket* client);
    void userBanned(int userId);
    void userKicked(int userId);
    void newMessageReceived(const QString& sender, const QString& message, bool isPrivate);

private slots:
    void handleNewConnection();
    void readClientData();
    void clientDisconnected();

private:
    QTcpServer* tcpServer;
    QMap<QTcpSocket*, int> clients;
    DatabaseHandler* dbHandler;
};

#endif // SERVERADMIN_H