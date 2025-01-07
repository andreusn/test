#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>

class User {
public:
    std::string _name;
    std::string _login;
    std::string _pass;

    // Сохранение в файл
    void saveToFile(const std::string& filename) {
        std::ofstream outFile(filename, std::ios::binary);
        if (outFile.is_open()) {
            outFile.write(_name.c_str(), _name.size());
            outFile.put('\0');  // Для разделения строк
            outFile.write(_login.c_str(), _login.size());
            outFile.put('\0');
            outFile.write(_pass.c_str(), _pass.size());
            outFile.put('\0');
            outFile.close();

            // Устанавливаем права на файл (только для владельца)
            chmod(filename.c_str(), S_IRUSR | S_IWUSR);
        } else {
            std::cerr << "Ошибка при открытии файла для записи" << std::endl;
        }
    }

    // Чтение из файла
    void loadFromFile(const std::string& filename) {
        std::ifstream inFile(filename, std::ios::binary);
        if (inFile.is_open()) {
            std::getline(inFile, _name, '\0');
            std::getline(inFile, _login, '\0');
            std::getline(inFile, _pass, '\0');
            inFile.close();
        } else {
            std::cerr << "Ошибка при открытии файла для чтения" << std::endl;
        }
    }
};

class Message {
public:
    std::string _text;
    std::string _sender;
    std::string _receiver;

    // Сохранение в файл
    void saveToFile(const std::string& filename) {
        std::ofstream outFile(filename, std::ios::binary);
        if (outFile.is_open()) {
            outFile.write(_text.c_str(), _text.size());
            outFile.put('\0');  // Для разделения строк
            outFile.write(_sender.c_str(), _sender.size());
            outFile.put('\0');
            outFile.write(_receiver.c_str(), _receiver.size());
            outFile.put('\0');
            outFile.close();

            // Устанавливаем права на файл (только для владельца)
            chmod(filename.c_str(), S_IRUSR | S_IWUSR);
        } else {
            std::cerr << "Ошибка при открытии файла для записи" << std::endl;
        }
    }

    // Чтение из файла
    void loadFromFile(const std::string& filename) {
        std::ifstream inFile(filename, std::ios::binary);
        if (inFile.is_open()) {
            std::getline(inFile, _text, '\0');
            std::getline(inFile, _sender, '\0');
            std::getline(inFile, _receiver, '\0');
            inFile.close();
        } else {
            std::cerr << "Ошибка при открытии файла для чтения" << std::endl;
        }
    }
};

int main() {
    // Пример использования User
    User user;
    user._name = "Andreu Smith";
    user._login = "andreusmith";
    user._pass = "password123";
    user.saveToFile("user_data.dat");

    // Пример использования Message
    Message msg;
    msg._text = "Hello, how are you?";
    msg._sender = "andreusmith";
    msg._receiver = "kattyperry";
    msg.saveToFile("message_data.dat");

    // Чтение из файла
    User userFromFile;
    userFromFile.loadFromFile("user_data.dat");

    Message msgFromFile;
    msgFromFile.loadFromFile("message_data.dat");

    std::cout << "User from file: " << userFromFile._name << ", " << userFromFile._login << std::endl;
    std::cout << "Message from file: " << msgFromFile._text << " from " << msgFromFile._sender << " to " << msgFromFile._receiver << std::endl;

    return 0;
}