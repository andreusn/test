#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sstream>
#include <vector>
#include <map>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "odbc32.lib")

#define PORT 8888
#define BUFFER_SIZE 1024

// Функция для выполнения SQL-запросов через ODBC
void executeSQL(SQLHDBC hdbc, const std::string& query) {
    SQLHSTMT hstmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
    SQLExecDirect(hstmt, (SQLCHAR*)query.c_str(), SQL_NTS);
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
}

// Функция для получения результатов SQL-запросов
std::string fetchSQLResult(SQLHDBC hdbc, const std::string& query) {
    SQLHSTMT hstmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
    SQLExecDirect(hstmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    std::string result;
    SQLCHAR buffer[BUFFER_SIZE];
    while (SQLFetch(hstmt) == SQL_SUCCESS) {
        SQLGetData(hstmt, 1, SQL_C_CHAR, buffer, BUFFER_SIZE, nullptr);
        result += reinterpret_cast<char*>(buffer);
        result += "\n";
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return result;
}

// Функция для обработки клиентских запросов
void handleClient(SOCKET clientSocket, SQLHDBC hdbc) {
    char buffer[BUFFER_SIZE];
    int bytesReceived;

    while ((bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[bytesReceived] = '\0';
        std::string request(buffer);

        if (request.find("REGISTER") == 0) {
            // Регистрация пользователя
            std::string username = request.substr(9, request.find(' ', 9) - 9);
            std::string password = request.substr(9 + username.length() + 1);
            std::string query = "INSERT INTO users (username, password) VALUES ('" + username + "', '" + password + "')";
            executeSQL(hdbc, query);
            send(clientSocket, "REGISTER_SUCCESS", 16, 0);
        } else if (request.find("LOGIN") == 0) {
            // Авторизация пользователя
            std::string username = request.substr(6, request.find(' ', 6) - 6);
            std::string password = request.substr(6 + username.length() + 1);
            std::string query = "SELECT * FROM users WHERE username='" + username + "' AND password='" + password + "'";
            std::string result = fetchSQLResult(hdbc, query);

            if (!result.empty()) {
                send(clientSocket, "LOGIN_SUCCESS", 13, 0);
            } else {
                send(clientSocket, "LOGIN_FAILED", 12, 0);
            }
        } else if (request.find("SEND_MESSAGE") == 0) {
            // Отправка сообщения
            std::string username = request.substr(13, request.find(' ', 13) - 13);
            std::string message = request.substr(13 + username.length() + 1);
            std::string query = "INSERT INTO messages (user_id, message) VALUES ((SELECT id FROM users WHERE username='" + username + "'), '" + message + "')";
            executeSQL(hdbc, query);
            send(clientSocket, "MESSAGE_SENT", 12, 0);
        } else if (request.find("GET_HISTORY") == 0) {
            // Получение истории сообщений
            std::string query = "SELECT username, message, timestamp FROM messages JOIN users ON messages.user_id = users.id";
            std::string result = fetchSQLResult(hdbc, query);
            send(clientSocket, result.c_str(), result.length(), 0);
        }
    }

    closesocket(clientSocket);
}

int main() {
    // Инициализация Winsock
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Инициализация ODBC
    SQLHENV henv;
    SQLHDBC hdbc;
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
    SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
    SQLConnect(hdbc, (SQLCHAR*)"chat_db", SQL_NTS, nullptr, 0, nullptr, 0);

    // Создание сокета
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);

    std::cout << "Сервер запущен на порту " << PORT << std::endl;

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        std::thread(handleClient, clientSocket, hdbc).detach();
    }

    // Закрытие соединений
    SQLDisconnect(hdbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
    SQLFreeHandle(SQL_HANDLE_ENV, henv);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}