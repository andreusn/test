#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8888
#define SERVER_IP "127.0.0.1"

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    // Пример регистрации
    std::string registerRequest = "REGISTER user1 password1";
    send(clientSocket, registerRequest.c_str(), registerRequest.length(), 0);

    char buffer[1024];
    recv(clientSocket, buffer, 1024, 0);
    std::cout << "Ответ сервера: " << buffer << std::endl;

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}