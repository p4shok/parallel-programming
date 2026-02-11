# Компилятор и флаги
CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread
DEBUGFLAGS = -g -O0 -DDEBUG
RELEASEFLAGS = -O3 -DNDEBUG

# Директории
SRCDIR = .
BUILDDIR = build
BINDIR = $(BUILDDIR)/bin

# ВСЕ ПУТИ ВКЛЮЧЕНИЯ - добавляем корневую директорию и все поддиректории
INCLUDES = -I. \
           -I./utils \
           -I./utils/conf \
           -I./models \
           -I./models/impl \
           -I./logger \
           -I./logger/impl

# Исходные файлы - рекурсивный поиск
rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
CPP_SOURCES = $(call rwildcard, , *.cpp)
C_SOURCES = $(call rwildcard, , *.c)

# Объектные файлы с сохранением структуры папок
CPP_OBJECTS = $(patsubst %.cpp, $(BUILDDIR)/%.o, $(CPP_SOURCES))
C_OBJECTS = $(patsubst %.c, $(BUILDDIR)/%.o, $(C_SOURCES))
OBJECTS = $(CPP_OBJECTS) $(C_OBJECTS)

# Имя исполняемого файла
TARGET = $(BINDIR)/parallel_programming

# Дефолтная цель
all: release

# Релизная сборка
release: CXXFLAGS += $(RELEASEFLAGS)
release: setup $(TARGET)

# Дебажная сборка
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: setup $(TARGET)

# Создание директорий
setup:
	@mkdir -p $(BINDIR)
	@mkdir -p $(dir $(OBJECTS))

# Линковка
$(TARGET): $(OBJECTS)
	$(CXX) $^ -o $@ $(CXXFLAGS)

# Правило для всех cpp файлов с сохранением структуры
$(BUILDDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Правило для всех c файлов
$(BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Очистка
clean:
	rm -rf $(BUILDDIR)

# Запуск программы
run: release
	./$(TARGET)

# Показать все исходники и объектные файлы
show:
	@echo "C++ файлы:"
	@echo $(CPP_SOURCES) | tr ' ' '\n'
	@echo "\nОбъектные файлы:"
	@echo $(OBJECTS) | tr ' ' '\n'

.PHONY: all clean setup run show release debug