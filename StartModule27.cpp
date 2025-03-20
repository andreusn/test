#include <thread>
#include <vector>

void logMessages(Logger& logger, int threadId) {
    for (int i = 0; i < 5; ++i) {
        std::string message = "Поток " + std::to_string(threadId) + ": Сообщение " + std::to_string(i);
        logger.writeLog(message);
    }
}

int main() {
    Logger logger("log.txt");

    std::vector<std::thread> threads;
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back(logMessages, std::ref(logger), i);
    }

    for (auto& t : threads) {
        t.join();
    }

    // Чтение логов
    std::cout << "Чтение логов:" << std::endl;
    std::string line;
    while (!(line = logger.readLog()).empty()) {
        std::cout << line << std::endl;
    }

    return 0;
}