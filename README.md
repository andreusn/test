# 1) Консольный чат на C++

## Описание проекта
Проект реализует консольный чат с регистрацией пользователей, регистрацией, отправкой сообщений между пользователями и общий чат.

## Описание классов
- User: Класс для хранения данных пользователя.
- Message: Класс для хранения сообщений.
- Chat: Основной класс для управления чатом и сообщениями.

## Функции программы
- Регистрация нового пользователя.
- Вход пользователя в систему.
- Отправка сообщений конкретному пользователю и всем пользователям.
- Просмотр сообщений.


# 2) Генерация файла на C++

- Функция generateFile генерирует файл заданного размера (2 ГБ) и заполняет его случайными числами.
- Функция sortAndSaveChunk сортирует подмассив чисел (кусок исходного файла) и сохраняет его в отдельный временный файл.
- Функция mergeSortedFiles объединяет все временные файлы в один отсортированный выходной файл с помощью кучи минимальных элементов для многопутевого слияния.
- Основная программа сначала генерирует большой файл, затем выполняет внешнюю сортировку и объединяет все временные файлы в итоговый отсортированный файл.
## Пояснение этапов внешней сортировки:
- Частичная сортировка: загружается кусок файла, который можно поместить в оперативную память, сортируется и записывается как временный файл.
- Слияние: во время слияния загружаются первые элементы каждого временного файла и, используя кучу, находим минимальный элемент для записи в итоговый файл.


# 3) Проверка кратности на C++

## Проверка на кратность 
Cначала проверяем, что \( K \) больше нуля и что длина строки делится на \( K \) (str.length() % K != 0).
   
## Извлечение шаблона 
Извлекаем первую подстроку длиной \( K \) (это будет "шаблон", который мы будем сравнивать с остальными подстроками).
   
## Проверка подстрок
Далее, мы проходим по строке с шагом \( K \) и сравниваем каждую подстроку длиной \( K \) с шаблоном. Если хотя бы одна подстрока не совпадает с шаблоном, возвращаем false.

## Возврат результата
Если все подстроки совпадают, то строка является \( K \)-периодичной, и мы возвращаем true.


# 4) Aвтодопление введенного слова на C++

1. Структура Trie:
   - TrieNode представляет узел дерева с массивом указателей children (по числу букв алфавита) и булевым флагом isEndOfWord, указывающим на конец слова.
2. Функции в Trie:
   - insert: добавляет слово в дерево.
   - getSuggestions: принимает префикс, находит соответствующий узел в дереве и вызывает рекурсивную функцию findWords для поиска всех слов, начинающихся с данного префикса.

3. Словарь:
   - Предварительно заданный массив слов используется для построения дерева.

4. Цикл работы:
   - Пользователь вводит префикс.
   - Программа возвращает список слов, начинающихся с этого префикса.
   - Если пользователь вводит exit, программа завершает выполнение.

# 4) Сетевой чат на Linux с использованием C++

### Инструкции по использованию:
1. Скомпилируйте оба файла:
   - g++ server.cpp -o server
   - g++ client.cpp -o client
   
2. Запустите сервер:
   - ./server
3. В другом терминале запустите клиент:
   - ./client
