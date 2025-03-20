#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <shared_mutex>

class Logger {
public:
    // Конструктор открывает файл логов
    Logger(const std::string& filename) : logFile(filename, std::ios::in | std::ios::out | std::ios::app) {
        if (!logFile.is_open()) {
            throw std::runtime_error("Не удалось открыть файл логов.");
        }
    }

    // Деструктор закрывает файл логов
    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    // Метод для записи строки в лог
    void writeLog(const std::string& message) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        logFile << message << std::endl;
    }

    // Метод для чтения строки из лога
    std::string readLog() {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        std::string line;
        if (std::getline(logFile, line)) {
            return line;
        }
        return ""; // Если файл пуст или достигнут конец
    }

private:
    std::fstream logFile; // Файл логов
    std::shared_mutex mutex_; // Мьютекс для защиты доступа к файлу
};