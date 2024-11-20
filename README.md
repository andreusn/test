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
