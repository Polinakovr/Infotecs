# Компилятор
CXX = g++

# Флаги компиляции
CXXFLAGS = -Wall -Wextra -std=c++17 -pthread

# Исходные файлы
SRC_LIB = lib/library.cpp
SRC_PROG1 = program1/program_client.cpp
SRC_PROG2 = program2/programm_server.cpp

# Объектные файлы
OBJ_LIB = $(SRC_LIB:.cpp=.o)
OBJ_PROG1 = $(SRC_PROG1:.cpp=.o)
OBJ_PROG2 = $(SRC_PROG2:.cpp=.o)

# Цели
all: libprogram.so programma_1 programma_2

# Сборка динамической библиотеки
libprogram.so: $(OBJ_LIB)
	$(CXX) -shared -o $@ $^

# Сборка программы №1
programma_1: $(OBJ_PROG1) libprogram.so
	$(CXX) $(CXXFLAGS) -o $@ $^ -L. -lprogram -pthread -Wl,-rpath,.

# Сборка программы №2
programma_2: $(OBJ_PROG2) libprogram.so
	$(CXX) $(CXXFLAGS) -o $@ $^ -L. -lprogram -Wl,-rpath,.

# Компиляция исходных файлов в объектные
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка
clean:
	rm -f $(OBJ_LIB) $(OBJ_PROG1) $(OBJ_PROG2) libprogram.so programma_1 programma_2

.PHONY: all clean