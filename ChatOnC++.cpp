#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
using namespace std;

// Класс для хранения данных пользователя
class User {
private:
    string login;
    string password;
    string name;

public:
    User(const string& login, const string& password, const string& name)
        : login(login), password(password), name(name) {}

    string getLogin() const { return login; }
    string getName() const { return name; }

    bool checkPassword(const string& pass) const {
        return password == pass;
    }
};

// Класс для сообщений
class Message {
private:
    string from;
    string to;
    string text;

public:
    Message(const string& from, const string& to, const string& text)
        : from(from), to(to), text(text) {}

    string getFrom() const { return from; }
    string getTo() const { return to; }
    string getText() const { return text; }
};

// Основной класс чата
class Chat {
private:
    map<string, User> users;
    vector<Message> messages;
    User* currentUser = nullptr;  // Текущий пользователь после входа

public:
    // Регистрация пользователя
    void registerUser(const string& login, const string& password, const string& name) {
        if (users.find(login) != users.end()) {
            throw runtime_error("Пользователь с таким логином уже существует.");
        }
        users[login] = User(login, password, name);
        cout << "Пользователь успешно зарегистрирован." << endl;
    }

    // Вход пользователя
    void loginUser(const string& login, const string& password) {
        auto it = users.find(login);
        if (it == users.end()) {
            throw runtime_error("Неверный логин.");
        }
        if (!it->second.checkPassword(password)) {
            throw runtime_error("Неверный пароль.");
        }
        currentUser = &it->second;
        cout << "Вы вошли как " << currentUser->getName() << endl;
    }

    // Выход пользователя
    void logoutUser() {
        if (currentUser) {
            cout << "Вы вышли из системы." << endl;
            currentUser = nullptr;
        }
        else {
            cout << "Никто не вошел в систему." << endl;
        }
    }

    // Отправка сообщения конкретному пользователю
    void sendMessageToUser(const string& to, const string& text) {
        if (!currentUser) {
            throw runtime_error("Необходимо войти в систему для отправки сообщений.");
        }
        auto it = users.find(to);
        if (it == users.end()) {
            throw runtime_error("Пользователь не найден.");
        }
        messages.push_back(Message(currentUser->getLogin(), to, text));
        cout << "Сообщение отправлено пользователю " << it->second.getName() << endl;
    }

    // Отправка сообщения всем пользователям
    void sendMessageToAll(const string& text) {
        if (!currentUser) {
            throw runtime_error("Необходимо войти в систему для отправки сообщений.");
        }
        for (const auto& user : users) {
            if (user.first != currentUser->getLogin()) {
                messages.push_back(Message(currentUser->getLogin(), user.first, text));
            }
        }
        cout << "Сообщение отправлено всем пользователям." << endl;
    }

    // Показать все сообщения для текущего пользователя
    void showMessages() const {
        if (!currentUser) {
            throw runtime_error("Необходимо войти в систему для просмотра сообщений.");
        }
        bool hasMessages = false;
        for (const auto& message : messages) {
            if (message.getTo() == currentUser->getLogin() || message.getFrom() == currentUser->getLogin()) {
                cout << "От: " << message.getFrom() << " Кому: " << message.getTo() << " Текст: " << message.getText() << endl;
                hasMessages = true;
            }
        }
        if (!hasMessages) {
            cout << "Сообщений нет." << endl;
        }
    }
};
int main() {
    Chat chat;
    try {
        int choice;
        while (true) {
            cout << "\n1. Регистрация\n2. Вход\n3. Отправить сообщение пользователю\n4. Отправить сообщение всем\n5. Показать сообщения\n6. Выход\n7. Выход из программы\n";
            cout << "Ваш выбор: ";
            cin >> choice;

            if (choice == 1) {
                string login, password, name;
                cout << "Введите логин: ";
                cin >> login;
                cout << "Введите пароль: ";
                cin >> password;
                cout << "Введите имя: ";
                cin >> name;
                chat.registerUser(login, password, name);
            }
            else if (choice == 2) {
                string login, password;
                cout << "Введите логин: ";
                cin >> login;
                cout << "Введите пароль: ";
                cin >> password;
                chat.loginUser(login, password);
            }
            else if (choice == 3) {
                string to, text;
                cout << "Введите логин получателя: ";
                cin >> to;
                cout << "Введите сообщение: ";
                cin.ignore();
                getline(cin, text);
                chat.sendMessageToUser(to, text);
            }
            else if (choice == 4) {
                string text;
                cout << "Введите сообщение для всех: ";
                cin.ignore();
                getline(cin, text);
                chat.sendMessageToAll(text);
            }
            else if (choice == 5) {
                chat.showMessages();
            }
            else if (choice == 6) {
                chat.logoutUser();
            }
            else if (choice == 7) {
                cout << "Выход из программы..." << endl;
                break;
            }
            else {
                cout << "Неверный выбор. Попробуйте снова." << endl;
            }
        }
    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }

    return 0;
}