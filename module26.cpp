#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <algorithm>

// Функция для вычисления суммы части массива
void partial_sum(const std::vector<int>& arr, int start, int end, int& result) {
    result = 0;
    for (int i = start; i < end; ++i) {
        result += arr[i];
    }
}

int main() {
    int N, M;
    std::cout << "Введите размер массива (N): ";
    std::cin >> N;
    std::cout << "Введите количество частей (M): ";
    std::cin >> M;

    // Генерация массива случайных чисел
    std::vector<int> arr(N);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    for (int& num : arr) {
        num = dis(gen);
    }

    // Вывод массива для проверки
    std::cout << "Массив: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Размер каждой части
    int part_size = N / M;
    std::vector<std::thread> threads;
    std::vector<int> partial_sums(M, 0);

    // Создание и запуск потоков
    for (int i = 0; i < M; ++i) {
        int start = i * part_size;
        int end = (i == M - 1) ? N : start + part_size; // Последняя часть может быть больше
        threads.emplace_back(partial_sum, std::ref(arr), start, end, std::ref(partial_sums[i]));
    }

    // Ожидание завершения всех потоков
    for (auto& t : threads) {
        t.join();
    }

    // Вычисление общей суммы
    int total_sum = 0;
    for (int sum : partial_sums) {
        total_sum += sum;
    }

    std::cout << "Общая сумма элементов массива: " << total_sum << std::endl;

    return 0;
}