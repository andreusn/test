#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>
#include <QDateTime>
#include <QDebug>

struct User {
    int id;
    QString username;
    QString password;
    QString email;
    bool isBanned;
    bool isOnline;
    QDateTime lastSeen;
};

struct Message {
    int id;
    int senderId;
    int receiverId;
    QString content;
    QDateTime timestamp;
    bool isPrivate;
};

class DatabaseHandler : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseHandler(QObject* parent = nullptr);
    ~DatabaseHandler();

    bool connectToDatabase(const QString& host, const QString& dbName,
        const QString& user, const QString& password);
    void closeConnection();

    QVector<User> getAllUsers();
    QVector<Message> getAllMessages();
    QVector<Message> getPrivateMessages();
    QVector<Message> getPublicMessages();

    bool banUser(int userId, bool banStatus);
    bool disconnectUser(int userId);
    bool deleteMessage(int messageId);

    User getUserById(int userId);

private:
    QSqlDatabase db;
};

#endif // DATABASEHANDLER_H