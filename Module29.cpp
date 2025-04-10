void insertIntoMiddle(int value, int pos) {
    //создаем новый узел
    Node* newNode = new Node;
    newNode->value = value;
    newNode->next = nullptr;
    newNode->node_mutex = new std::mutex;

    Node* prev = nullptr;
    Node* curr = nullptr;
    
    //блокаем всю очередь на время поиска начальной позиции
    queue_mutex->lock();
    
    try {
        prev = head;
        prev->node_mutex->lock();
        curr = prev->next;
        if (curr) curr->node_mutex->lock();
        
        queue_mutex->unlock(); //разблокируем очередь работаем только с узлами
        
        int counter = 1; //позиции считаем с 1 (head - позиция 0)
        
        
        while (curr && counter < pos) {
            // Перемещаем блокировки
            Node* oldPrev = prev;
            prev = curr;
            curr = curr->next;
            
            oldPrev->node_mutex->unlock();
            if (curr) curr->node_mutex->lock();
            
            counter++;
        }
        
        //вставляем узел
        newNode->next = curr;
        prev->next = newNode;
        
        // разблокируем оставшиеся узлы
        prev->node_mutex->unlock();
        if (curr) curr->node_mutex->unlock();
    }
    catch (...) {
        //в случае исключения обязательно освобождаем все блокировки
        queue_mutex->unlock();
        if (prev && prev->node_mutex) prev->node_mutex->unlock();
        if (curr && curr->node_mutex) curr->node_mutex->unlock();
        throw;
    }
}