#include <iostream>
#include <mutex>
#include <thread>
#include <array>

struct Node
{
    int value;
    Node* next;
    std::mutex node_mutex;
};

class FineGrainedQueue
{
public:
    Node* head;
    std::mutex queue_mutex;

    FineGrainedQueue() : head(nullptr) {}

    void insertIntoMiddle(int value, int pos)
    {
        Node* newNode = new Node{value, nullptr, {}};
        
        queue_mutex.lock();
        Node* current = head;
        queue_mutex.unlock();
        
        if (!current) {
            queue_mutex.lock();
            head = newNode;
            queue_mutex.unlock();
            return;
        }
        
        int currentPos = 0;
        Node* prev = nullptr;
        
        while (current && currentPos < pos) {
            prev = current;
            current = current->next;
            currentPos++;
            
            if (!current) break;
        }
        
        if (prev) {
            std::unique_lock<std::mutex> prevLock(prev->node_mutex);
            std::unique_lock<std::mutex> currentLock;
            if (current) {
                currentLock = std::unique_lock<std::mutex>(current->node_mutex);
            }
            
            newNode->next = current;
            prev->next = newNode;
        }
    }

    void print()
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        Node* current = head;
        while (current) {
            std::cout << current->value << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

int main([[maybe_unused]] int argc, [[maybe_unused]] const char** argv)
{
    FineGrainedQueue queue;

    const int max_elems = 10;
    for (size_t i = 0; i < max_elems; ++i)
    {
        queue.insertIntoMiddle(i, i); 
    }

    const int num_threads = 5;
    std::array<std::thread, num_threads> threads;

    for (size_t i = 0; i < threads.size(); ++i)
    {
        threads[i] = std::thread([&queue, i]() { queue.insertIntoMiddle((i + 1) * 100, 5); });
    }

    for (auto& t : threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }

    queue.print();
    return 0;
}
