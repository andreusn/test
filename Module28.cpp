#include <vector>
#include <future>
#include <algorithm>
#include <iostream>

//функция для слияния двух отсортированных подмассивов
template<typename T>
void merge(std::vector<T>& arr, size_t left, size_t mid, size_t right) {
    std::vector<T> temp(right - left + 1);
    size_t i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    while (j <= right) {
        temp[k++] = arr[j++];
    }

    for (i = left, k = 0; i <= right; ++i, ++k) {
        arr[i] = temp[k];
    }
}

// рекурсивная функция сортировки слиянием с  многопоточностью
template<typename T>
void merge_sort(std::vector<T>& arr, size_t left, size_t right, bool make_thread = true) {
    if (left >= right) {
        return;
    }

    size_t mid = left + (right - left) / 2;

    // если элементов достаточно много и разрешено создание потоков
    if (make_thread && (right - left > 10000)) {
        //Запускаем левую часть в отдельном потоке
        auto future = std::async(std::launch::async, [&]() {
            merge_sort(arr, left, mid, false);  // Внутри рекурсии новые потоки не создаем
        });

        //правую часть обрабатываем в текущем потоке
        merge_sort(arr, mid + 1, right, false);

        //дожидаемся завершения асинхронной задачи
        future.get();

    } else {
        //синхронная обработка обеих частей
        merge_sort(arr, left, mid, false);
        merge_sort(arr, mid + 1, right, false);
    }

    //слияние отсортированных подмассивов
    merge(arr, left, mid, right);
}

template<typename T>
void parallel_merge_sort(std::vector<T>& arr) {
    if (!arr.empty()) {
        merge_sort(arr, 0, arr.size() - 1);
    }
}

int main() {
    std::vector<int> data = {5, 3, 8, 1, 2, 7, 4, 6, 9, 0};
    
    std::cout << "До сортировки: ";
    for (int num : data) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    parallel_merge_sort(data);

    std::cout << "После сортировки:  ";
    for (int num : data) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}