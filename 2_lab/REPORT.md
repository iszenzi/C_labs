# Лабораторная работа №2 (Cортировка и поиск)

## Цель работы
Составить программу на языке Си с использованием процедур и функций для сортировки таблицы 
методом Шелла и двоичного поиска по ключу в таблице. Структура данных: вещественный ключ + хранение вместе.

## Идея, метод, алгоритм решения задачи
- Структура записи: пара ключ (число с плавающей точкой) + указатель на C‑строку.
- Чтение строк производится функцией с динамическим буфером, увеличивающимся средствами `realloc`, чтобы поддержать произвольную длину.
- Файл формата: первая строка — число записей N, далее N строк вида `key <пробел> value` (значение — остаток строки).
- Сортировка: Shell sort по возрастанию ключа.
- Поиск: итеративный двоичный поиск по ключу, возвращающий индекс найденной записи или `-1`.

## Сценарий выполнения работы
1. Описал структуру записи  и прототипы в заголовочном файле.
2. Реализовал функции для чтения таблицы и освобождения памяти.
3. Реализовал сортировку Шелла  и бинарный поиск в модуле.
4. Сделал простой CLI в с меню: загрузить файл, вывести таблицу, отсортировать, искать по ключу, выйти.
5. Написал тесты для проверки чтения, сортировки и поиска.

## Структура репозитория
```
С_LABS/
├── 2_lab/
    ├── Makefile            # Сборка и запуск
    ├── main.c              # CLI для работы с таблицей
    ├── table.c             # Реализация чтения, сортировки, поиска
    ├── table.h             # Структура и прототипы
    ├── test_table.c        # Тесты
    ├── sorted.txt          # Тестовый файл (отсортирован)
    ├── reverse.txt         # Тестовый файл (обратный порядок)
    └── random.txt          # Тестовый файл (случайный порядок)
```

## Запуск
Сборка и запуск (в каталоге `2_lab`):

```
mingw32-make
```

## Пример работы
```
Menu:
Current file: (none)
1. Load table from file
2. Print table
3. Shell sort
4. Binary search
5. Exit
Enter command 1
Enter filename random.txt
Menu:
Current file: random.txt
1. Load table from file
2. Print table
3. Shell sort
4. Binary search
5. Exit
Enter command 3
Before sort:
1 1.3 Smack that give me some more
2 0.2 Akon, Slim Shady
3 1 Smack that 'till you get sore
4 0.1 Shady, Konvict, Upfront
5 1.5 Smack that oh-oh!
6 0.9 Smack that give me some more
7 0.4 I feel you creeping, I can see it from my shadow
8 0.7 And possibly bend you over look back and watch me
9 1.2 Smack that all on the floor
10 0.6 Maybe go to my place and just kick it like TaeBo
11 0.3 I see the one, because she be that lady! Hey!
12 1.4 Smack that 'till you get sore
13 0.8 Smack that all on the floor
14 0.5 Wanna jump up in my Lamborghini Gallardo
15 1.1 Smack that oh-oh!
After sort:
1 0.1 Shady, Konvict, Upfront
2 0.2 Akon, Slim Shady
3 0.3 I see the one, because she be that lady! Hey!
4 0.4 I feel you creeping, I can see it from my shadow
5 0.5 Wanna jump up in my Lamborghini Gallardo
6 0.6 Maybe go to my place and just kick it like TaeBo
7 0.7 And possibly bend you over look back and watch me
8 0.8 Smack that all on the floor
9 0.9 Smack that give me some more
10 1 Smack that 'till you get sore
11 1.1 Smack that oh-oh!
12 1.2 Smack that all on the floor
13 1.3 Smack that give me some more
14 1.4 Smack that 'till you get sore
15 1.5 Smack that oh-oh!
Menu:
Current file: random.txt
1. Load table from file
2. Print table
3. Shell sort
4. Binary search
5. Exit
Enter command 4
Enter key 0.8
Found: 0.8 Smack that all on the floor
Menu:
Current file: random.txt
1. Load table from file
2. Print table
3. Shell sort
4. Binary search
5. Exit
Enter command 4
Enter key 0.9
Found: 0.9 Smack that give me some more
Menu:
Current file: random.txt
1. Load table from file
2. Print table
3. Shell sort
4. Binary search
5. Exit
Enter command 5
```

## Вывод
Разработана программа для работы с табличной структурой данных: каждая запись — числовой ключ и динамическая строка. Реализованы безопасное чтение строк произвольной длины, загрузка таблицы из файла, сортировка методом Шелла и двоичный поиск по ключу. Написаны модульные тесты на assert и настроен Makefile для сборки и запуска.
