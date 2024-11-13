#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string>
#include <queue>

// Размер буфера для чтения данных (например, 100 МБ)
const size_t BUFFER_SIZE = 100 * 1024 * 1024;

// Функция для генерации большого файла
void generateFile(const std::string& filename, size_t fileSize) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "Не удалось открыть файл для записи.\n";
        return;
    }

    srand(time(0));
    size_t totalBytesWritten = 0;

    while (totalBytesWritten < fileSize) {
        int num = rand(); // Генерируем случайное число
        outFile.write(reinterpret_cast<char*>(&num), sizeof(num));
        totalBytesWritten += sizeof(num);
    }

    outFile.close();
}

// Функция для сортировки части данных и записи их во временный файл
std::string sortAndSaveChunk(std::vector<int>& chunk, int chunkIndex) {
    std::sort(chunk.begin(), chunk.end()); // Сортируем кусок в памяти

    std::string tempFilename = "temp_" + std::to_string(chunkIndex) + ".bin";
    std::ofstream outFile(tempFilename, std::ios::binary);
    for (int num : chunk) {
        outFile.write(reinterpret_cast<char*>(&num), sizeof(num));
    }
    outFile.close();

    return tempFilename;
}

// Функция для этапа слияния отсортированных временных файлов
void mergeSortedFiles(const std::vector<std::string>& sortedChunks, const std::string& outputFilename) {
    std::ofstream outFile(outputFilename, std::ios::binary);
    if (!outFile) {
        std::cerr << "Не удалось открыть выходной файл для записи.\n";
        return;
    }

    // Открываем каждый временный файл для чтения
    std::vector<std::ifstream> chunkFiles;
    for (const auto& chunkFile : sortedChunks) {
        chunkFiles.emplace_back(chunkFile, std::ios::binary);
    }

    // Структура для хранения текущих чисел из каждого файла и их индексов
    using Elem = std::pair<int, int>; // {значение, индекс_файла}
    auto cmp = [](const Elem& a, const Elem& b) { return a.first > b.first; };
    std::priority_queue<Elem, std::vector<Elem>, decltype(cmp)> minHeap(cmp);

    // Инициализируем кучу первыми элементами из каждого файла
    for (size_t i = 0; i < chunkFiles.size(); ++i) {
        int value;
        if (chunkFiles[i].read(reinterpret_cast<char*>(&value), sizeof(value))) {
            minHeap.emplace(value, i);
        }
    }

    // Слияние данных
    while (!minHeap.empty()) {
        auto [minValue, fileIndex] = minHeap.top();
        minHeap.pop();

        // Записываем минимальный элемент в выходной файл
        outFile.write(reinterpret_cast<char*>(&minValue), sizeof(minValue));

        // Считываем следующий элемент из того же файла
        int nextValue;
        if (chunkFiles[fileIndex].read(reinterpret_cast<char*>(&nextValue), sizeof(nextValue))) {
            minHeap.emplace(nextValue, fileIndex);
        }
    }

    // Закрываем все файлы
    for (auto& file : chunkFiles) {
        file.close();
    }
    outFile.close();
}

int main() {
    const std::string inputFilename = "bigfile.bin";
    const std::string outputFilename = "sorted_bigfile.bin";
    const size_t fileSize = 2ULL * 1024 * 1024 * 1024; // 2 GB

    // Генерируем исходный файл
    std::cout << "Генерация исходного файла...\n";
    generateFile(inputFilename, fileSize);

    // Этап разделения: сортируем части исходного файла и сохраняем их
    std::ifstream inFile(inputFilename, std::ios::binary);
    if (!inFile) {
        std::cerr << "Не удалось открыть входной файл.\n";
        return 1;
    }

    std::vector<std::string> sortedChunks;
    std::vector<int> buffer;
    int chunkIndex = 0;

    while (true) {
        buffer.clear();
        buffer.reserve(BUFFER_SIZE / sizeof(int));

        // Читаем часть данных в буфер
        int num;
        size_t bytesRead = 0;
        while (bytesRead < BUFFER_SIZE && inFile.read(reinterpret_cast<char*>(&num), sizeof(num))) {
            buffer.push_back(num);
            bytesRead += sizeof(num);
        }
    
if (buffer.empty()) break;

        // Сортируем буфер и сохраняем его как временный файл
        sortedChunks.push_back(sortAndSaveChunk(buffer, chunkIndex++));
    }

    inFile.close();

    // Этап слияния: объединяем отсортированные временные файлы
    std::cout << "Слияние временных файлов в выходной файл...\n";
    mergeSortedFiles(sortedChunks, outputFilename);

    // Удаляем временные файлы
    for (const auto& tempFile : sortedChunks) {
        std::remove(tempFile.c_str());
    }

    std::cout << "Сортировка завершена. Отсортированные данные сохранены в " << outputFilename << "\n";
    return 0;
}