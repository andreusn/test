#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Структура для хранения информации о сообщении
struct Message {
    string username;
    string text;

    Message(const string& user, const string& msg) : username(user), text(msg) {}
};

// Класс для чата
class Chat {
public:
    // Метод для добавления сообщения
    void addMessage(const string& user, const string& message) {
        messages.push_back(Message(user, message));
    }

    // Метод для вывода истории чата
    void printChat() const {
        for (const auto& msg : messages) {
            cout << msg.username << ": " << msg.text << endl;
        }
    }

    // Метод для получения сообщений
    const vector<Message>& getMessages() const {
        return messages;
    }

private:
    vector<Message> messages;  // Хранение сообщений
};

// Пример использования чата
int main() {
    Chat chat;
    
    // Добавление сообщений
    chat.addMessage("Alice", "Hello, how are you?");
    chat.addMessage("Bob", "I'm good, thanks! How about you?");
    chat.addMessage("Alice", "I'm doing great!");
    chat.addMessage("Bob", "That's awesome!");

    // Вывод истории чата
    cout << "Chat history:" << endl;
    chat.printChat();

    return 0;
}