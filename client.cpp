#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Создание сокета
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error\n";
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Преобразование адреса IPv4 из текста в бинарный формат
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported\n";
        return -1;
    }

    // Подключение к серверу
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed\n";
        return -1;
    }
    std::cout << "Подключено к серверу.\n";

    char buffer[1024] = {0};
    while (true) {
        // Отправка сообщения серверу
        std::string client_message;
        std::cout << "Вы: ";
        std::getline(std::cin, client_message);
        send(sock, client_message.c_str(), client_message.size(), 0);

        // Прием сообщения от сервера
        memset(buffer, 0, sizeof(buffer));
        int valread = read(sock, buffer, sizeof(buffer));
        if (valread <= 0) {
            std::cout << "Сервер отключился.\n";
            break;
        }
        std::cout << "Сервер: " << buffer << "\n";
    }

    close(sock);
    return 0;
}