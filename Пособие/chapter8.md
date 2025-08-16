Отлично! Переходим к **Этапу 8** — финальному: **сборке проекта с помощью `Makefile` и запуску всех проверок**.

---

## 📘 Этап 8: Сборка проекта с `Makefile` — как автоматизировать сборку, тесты и анализ

> 💡 В С ты, возможно, компилировал вручную:
> ```bash
> gcc -c matrix.c -o matrix.o
> gcc main.o matrix.o -o program
> ```
>
> В C++ проектах используется **`Makefile`** — файл, который автоматизирует:
> - Сборку библиотеки
> - Компиляцию тестов
> - Запуск тестов
> - Проверку памяти и стиля

---

### 🔧 8.1. Создаём `Makefile` в папке `src/`

```bash
touch Makefile
```

Открой `Makefile` и вставь:

```makefile
# src/Makefile
# Универсальный Makefile для C++ проекта с тестами

# === Определение ОС ===
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
    OS = LINUX
else ifeq ($(UNAME_S), Darwin)
    OS = MACOS
endif

# === Компилятор и флаги ===
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -I.
GTEST_LIB = -lgtest -lgtest_main -pthread

# === Пути ===
BUILD_DIR = ./build
LIB = ./lib/libs21_matrix.a

# Все .cpp файлы (кроме тестов)
SOURCES = $(wildcard ./*.cpp)
SOURCES := $(filter-out ./Makefile ./tests/%.cpp, $(SOURCES))
OBJECTS = $(SOURCES:./%.cpp=$(BUILD_DIR)/%.o)

# Тесты
TEST_SOURCES = $(wildcard ./tests/*.cpp)
TEST_OBJECTS = $(TEST_SOURCES:./tests/%.cpp=$(BUILD_DIR)/tests/%.o)

# === Цели ===
.PHONY: all test clean s21_matrix_oop.a format check-format

# Цель по умолчанию — сборка библиотеки
all: $(LIB)

# Сборка статической библиотеки
$(LIB): $(OBJECTS)
	@mkdir -p $(dir $@)
	ar rcs $@ $^
	@echo "✅ Static library created: $(LIB)"

# Компиляция исходников
$(BUILD_DIR)/%.o: ./%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "   [COMPILE] $<"

# Сборка и запуск тестов
test: $(LIB) $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/test $(TEST_OBJECTS) $(LIB) $(GTEST_LIB)
	@echo "🧪 Running unit tests..."
	$(BUILD_DIR)/test

$(BUILD_DIR)/tests/%.o: ./tests/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "   [TEST] $<"

# Очистка
clean:
	rm -rf $(BUILD_DIR) $(LIB)
	@echo "🧹 Clean completed"

# Сборка библиотеки (явная цель)
s21_matrix_oop.a: $(LIB)

# Форматирование кода
format:
	@echo "✨ Formatting code with clang-format..."
	clang-format -i *.cpp *.h tests/*.cpp

# Проверка форматирования
check-format:
	@echo "🔍 Checking code style..."
	clang-format --dry-run *.cpp *.h tests/*.cpp
```

---

### ✅ 8.2. Объяснение для новичка

| Строка | Что делает |
|-------|-----------|
| `CXX = g++` | Используем компилятор `g++` |
| `CXXFLAGS = -std=c++17` | Включаем стандарт C++17 |
| `ar rcs lib/libs21_matrix.a ...` | Создаём статическую библиотеку |
| `$(CXX) -o test ...` | Собираем исполняемый файл тестов |
| `make test` | Запускает тесты |
| `make clean` | Удаляет собранные файлы |

> ✅ `Makefile` — это **инструкция для автоматизации**, как скрипт, но мощнее.

---

### 🔧 8.3. Структура папок после сборки

После `make test`:

```
src/
├── build/
│   ├── *.o                 # объектные файлы
│   └── test                # исполняемый файл тестов
├── lib/
│   └── libs21_matrix.a     # статическая библиотека
├── Makefile
├── s21_matrix_oop.h
├── *.cpp
└── tests/*.cpp
```

---

### 🔧 8.4. Как использовать `Makefile`

#### 1. Собрать библиотеку
```bash
make
```
или
```bash
make s21_matrix_oop.a
```

#### 2. Запустить тесты
```bash
make test
```

#### 3. Очистить
```bash
make clean
```

#### 4. Отформатировать код
```bash
make format
```

#### 5. Проверить стиль
```bash
make check-format
```

---

### ✅ 8.5. Пример успешного запуска

```bash
make test
✅ Static library created: ./lib/libs21_matrix.a
   [TEST] tests/constructors_test.cpp
   [TEST] tests/accessors_test.cpp
   ...
🧪 Running unit tests...
[==========] Running 15 tests from 6 test suites.
[  PASSED  ] 15 tests.
```

---

### 📚 Что изучить дополнительно:

| Тема | Ресурс |
|------|--------|
| [GNU Make Manual](https://www.gnu.org/software/make/manual/make.html) | Полная документация |
| [Makefile syntax](https://en.wikipedia.org/wiki/Makefile) | Объяснение переменных, целей, шаблонов |
| [Pattern rules](https://www.gnu.org/software/make/manual/html_node/Pattern-Intro.html) | `%.o: %.cpp` — автоматизация |
| [Phony targets](https://www.gnu.org/software/make/manual/html_node/Phony-Targets.html) | `.PHONY: clean` — чтобы `clean` не мешал файлам |

---

### ✅ Проверка

Убедись, что `Makefile` создан:

```bash
ls -l Makefile
```

---

✅ **Этап 8 завершён.**  
Теперь ты умеешь:
- Автоматизировать сборку с `Makefile`.
- Собирать статическую библиотеку.
- Запускать тесты одной командой.
- Поддерживать чистый код с помощью `clang-format`.

---

## 🎉 Поздравляю!  
Ты прошёл полный путь от **С к C++**, от **идеи к рабочему проекту** с:
- ООП
- Управлением памятью
- Перегрузкой операторов
- Unit-тестами
- Автоматической сборкой

Этот проект — отличная основа для изучения:
- Шаблонов (`template <typename T>`)
- Умных указателей (`std::unique_ptr`)
- CMake
- CI/CD (GitHub Actions)

Если хочешь — могу помочь:
- Перевести проект на **CMake**
- Добавить **GitHub Actions**
- Создать **документацию Doxygen**

Продолжай в том же духе! 💪🚀