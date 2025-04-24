#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

class ThreadSafeQueue {
private:
    std::queue<int> queue;          //очередь данных
    std::mutex mtx;                // Мьютекс для синхронизации
    std::condition_variable cond;   //условная переменная для ожидания данных
    bool stop_flag = false;         //флаг остановки (для graceful shutdown)

public:
    //добавление элемента в очередь
    void push(int value) {
        std::unique_lock<std::mutex> lock(mtx);
        queue.push(value);
        cond.notify_one();  // Уведомляем один ждущий поток
    }

    //извлечение элемента из очереди (блокирующее)
    bool pop(int& value) {
        std::unique_lock<std::mutex> lock(mtx);
        
        //Ждём пока не появится элемент или не будет установлен флаг остановки
        cond.wait(lock, [this]() { 
            return !queue.empty() || stop_flag; 
        });

        if (stop_flag && queue.empty()) {
            return false;  //очередь пуста и поток должен завершиться
        }

        value = queue.front();
        queue.pop();
        return true;
    }

    //установка флага остановки (для завершения работы)
    void stop() {
        std::unique_lock<std::mutex> lock(mtx);
        stop_flag = true;
        cond.notify_all();  //уведомляем все ждущие потоки
    }
};

//поставщик данных
void producer(ThreadSafeQueue& q, int id) {
    for (int i = 0; i < 5; ++i) {
        int value = id * 100 + i;
        q.push(value);
        std::cout << "Producer " << id << " pushed: " << value << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
    }
}

//потребитель данных
void consumer(ThreadSafeQueue& q, int id) {
    int value;
    while (q.pop(value)) {
        std::cout << "Consumer " << id << " popped: " << value << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
    }
    std::cout << "Consumer " << id << " stopped." << std::endl;
}

int main() {
    ThreadSafeQueue q;
    
    std::thread producer1(producer, std::ref(q), 1);
    std::thread producer2(producer, std::ref(q), 2);
    std::thread consumer1(consumer, std::ref(q), 1);
    std::thread consumer2(consumer, std::ref(q), 2);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    q.stop();

    producer1.join();
    producer2.join();
    consumer1.join();
    consumer2.join();

    return 0;
}