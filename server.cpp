#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Создание сокета
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Привязка сокета к порту
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Прослушивание входящих подключений
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    std::cout << "Сервер запущен. Ожидание подключения...\n";

    // Принятие входящего подключения
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    std::cout << "Клиент подключился.\n";

    char buffer[1024] = {0};
    while (true) {
        // Прием сообщения от клиента
        memset(buffer, 0, sizeof(buffer));
        int valread = read(new_socket, buffer, sizeof(buffer));
        if (valread <= 0) {
            std::cout << "Клиент отключился.\n";
            break;
        }
        std::cout << "Клиент: " << buffer << "\n";

        // Отправка сообщения клиенту
        std::string server_message;
        std::cout << "Вы: ";
        std::getline(std::cin, server_message);
        send(new_socket, server_message.c_str(), server_message.size(), 0);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}