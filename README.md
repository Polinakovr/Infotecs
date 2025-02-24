# Задание: Две программы и динамическая библиотека

## Описание задания

Задание состоит из двух программ (`programma_1` и `programma_2`) и динамической библиотеки. Взаимодействие между программами реализовано через сокеты. Библиотека содержит три функции, которые выполняют определённые операции над входной строкой.

### Динамическая библиотека

Библиотека состоит из трёх функций:

1. **Функция 1**:
   - Сортирует элементы входной строки по убыванию.
   - Все чётные элементы заменяет на латинские буквы «КВ».
   - Ничего не возвращает.

2. **Функция 2**:
   - Рассчитывает и возвращает общую сумму всех элементов входной строки, которые являются численными значениями.

3. **Функция 3**:
   - Анализирует длину входной строки.
   - Если длина строки больше 2 символов и кратна 32, возвращает «истина».
   - В противном случае возвращает «ложь».

### Программа №1 (`program_client.cpp`)

Программа состоит из двух потоков и общего буфера:

1. **Поток 1**:
   - Принимает строку от пользователя.
   - Проверяет, что строка состоит только из цифр и не превышает 64 символа.
   - Передаёт строку на обработку **Функции 1**.
   - Помещает обработанную строку в общий буфер.
   - Ожидает дальнейшего ввода пользователя.

2. **Поток 2**:
   - Обрабатывает данные из общего буфера.
   - После получения данных буфер очищается.
   - Выводит данные на экран.
   - Передаёт данные **Функции 2**.
   - Передаёт результат в **Программу №2**.
   - Ожидает следующие данные.

**Примечания по Программе №1**:
- Взаимодействие потоков синхронизировано. Поток №2 не опрашивает общий буфер постоянно.
- Программа работает независимо от статуса **Программы №2**. Остановка **Программы №2** не влияет на ввод пользователя. При перезапуске **Программы №2** происходит переподключение.

### Программа №2 (`program_server.cpp`)

- Ожидает данные от **Программы №1**.
- Передаёт полученные данные **Функции 3**.
- Если **Функция 3** возвращает «истина», выводит сообщение о полученных данных.
- Если возвращается «ложь», выводит сообщение об ошибке.
- Продолжает ожидать данные.

**Примечания по Программе №2**:
- Программа работает независимо от статуса **Программы №1**. Остановка **Программы №1** не приводит к немедленным проблемам. При потере связи программа ожидает переподключения.

## Запуск программы

1. Скомпилируйте проект, выполнив команду:
   ```bash
   make
2. Запустите Программу №2:
   ./programma_2
3. В другом терминале запустить Программу №1
  ./programma_1
4. В Программе №1 введите строку, состоящую только из цифр (не более 64 символов). Результат будет выведен в обоих программах:
   - В Программе №1 — обработанная строка.
   - В Программе №2 — результат анализа строки.
   
