#include "databasehandler.h"
#include <QDebug>
using namespace Qt;

DatabaseHandler::DatabaseHandler(QObject* parent) : QObject(parent)
{
}

DatabaseHandler::~DatabaseHandler()
{
    closeConnection();
}

bool DatabaseHandler::connectToDatabase(const QString& host, const QString& dbName,
    const QString& user, const QString& password)
{
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("messenger_database");
    db.setUserName("user_admin");
    db.setPassword("password_bababoi");

    if (!db.open()) {
        qDebug() << "Database connection error:" << db.lastError().text();
        return false;
    }
    return true;
}

void DatabaseHandler::closeConnection()
{
    if (db.isOpen()) {
        db.close();
    }
}

QVector<User> DatabaseHandler::getAllUsers()
{
    QVector<User> users;
    QSqlQuery query("SELECT id, username, email, is_banned, is_online, last_seen FROM users");

    while (query.next()) {
        User user;
        user.id = query.value(0).toInt();
        user.username = query.value(1).toString();
        user.email = query.value(2).toString();
        user.isBanned = query.value(3).toBool();
        user.isOnline = query.value(4).toBool();
        user.lastSeen = query.value(5).toDateTime();
        users.append(user);
    }

    return users;
}

QVector<Message> DatabaseHandler::getAllMessages()
{
    QVector<Message> messages;
    QSqlQuery query("SELECT id, sender_id, receiver_id, content, timestamp, is_private FROM messages ORDER BY timestamp DESC");

    while (query.next()) {
        Message msg;
        msg.id = query.value(0).toInt();
        msg.senderId = query.value(1).toInt();
        msg.receiverId = query.value(2).toInt();
        msg.content = query.value(3).toString();
        msg.timestamp = query.value(4).toDateTime();
        msg.isPrivate = query.value(5).toBool();
        messages.append(msg);
    }

    return messages;
}

QVector<Message> DatabaseHandler::getPrivateMessages()
{
    QVector<Message> messages;
    QSqlQuery query("SELECT id, sender_id, receiver_id, content, timestamp FROM messages WHERE is_private = true ORDER BY timestamp DESC");

    while (query.next()) {
        Message msg;
        msg.id = query.value(0).toInt();
        msg.senderId = query.value(1).toInt();
        msg.receiverId = query.value(2).toInt();
        msg.content = query.value(3).toString();
        msg.timestamp = query.value(4).toDateTime();
        msg.isPrivate = true;
        messages.append(msg);
    }

    return messages;
}

QVector<Message> DatabaseHandler::getPublicMessages()
{
    QVector<Message> messages;
    QSqlQuery query("SELECT id, sender_id, content, timestamp FROM messages WHERE is_private = false ORDER BY timestamp DESC");

    while (query.next()) {
        Message msg;
        msg.id = query.value(0).toInt();
        msg.senderId = query.value(1).toInt();
        msg.receiverId = -1;
        msg.content = query.value(2).toString();
        msg.timestamp = query.value(3).toDateTime();
        msg.isPrivate = false;
        messages.append(msg);
    }

    return messages;
}

bool DatabaseHandler::banUser(int userId, bool banStatus)
{
    QSqlQuery query;
    query.prepare("UPDATE users SET is_banned = ? WHERE id = ?");
    query.addBindValue(banStatus);
    query.addBindValue(userId);

    return query.exec();
}

bool DatabaseHandler::disconnectUser(int userId)
{
    QSqlQuery query;
    query.prepare("UPDATE users SET is_online = false, last_seen = NOW() WHERE id = ?");
    query.addBindValue(userId);

    return query.exec();
}

bool DatabaseHandler::deleteMessage(int messageId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM messages WHERE id = ?");
    query.addBindValue(messageId);

    return query.exec();
}

User DatabaseHandler::getUserById(int userId)
{
    User user;
    QSqlQuery query;
    query.prepare("SELECT id, username, email, is_banned, is_online, last_seen FROM users WHERE id = ?");
    query.addBindValue(userId);

    if (query.exec() && query.next()) {
        user.id = query.value(0).toInt();
        user.username = query.value(1).toString();
        user.email = query.value(2).toString();
        user.isBanned = query.value(3).toBool();
        user.isOnline = query.value(4).toBool();
        user.lastSeen = query.value(5).toDateTime();
    }

    return user;
}