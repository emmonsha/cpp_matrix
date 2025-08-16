# src/Makefile
# Универсальный Makefile для Linux и macOS с автоопределением GTest

# === Определение ОС ===
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
    OS = LINUX
else ifeq ($(UNAME_S), Darwin)
    OS = MACOS
else
    $(error Unsupported OS: $(UNAME_S))
endif

# === Компилятор и флаги ===
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -I.

# === Пути ===
BUILD_DIR = ./build
LIB = ./lib/libs21_matrix.a

# Все .cpp в src, кроме тестов и Makefile
SOURCES = $(wildcard ./*.cpp)
SOURCES := $(filter-out ./Makefile ./tests/%.cpp, $(SOURCES))
OBJECTS = $(SOURCES:./%.cpp=$(BUILD_DIR)/%.o)

# Тесты
TEST_SOURCES = $(wildcard ./tests/*.cpp)
TEST_OBJECTS = $(TEST_SOURCES:./tests/%.cpp=$(BUILD_DIR)/tests/%.o)

# === Настройка GTest в зависимости от ОС ===

# По умолчанию
GTEST_LIB =
GTEST_INC =

# === Linux: используем pkg-config, если доступен ===
ifeq ($(OS), LINUX)
    # Проверим, установлен ли pkg-config и доступен ли gtest
    ifshell = $(shell pkg-config --exists gtest && echo yes)
    ifeq ($(ifshell), yes)
        GTEST_LIB := $(shell pkg-config --libs gtest gtest_main)
        GTEST_INC := $(shell pkg-config --cflags gtest gtest_main)
    else
        # Резерв: стандартные флаги
        GTEST_LIB := -lgtest -lgtest_main -pthread
        GTEST_INC :=
    endif
endif

# === macOS: используем Homebrew ===
ifeq ($(OS), MACOS)
    # Определим префикс Homebrew
    BREW_PREFIX := $(shell brew --prefix 2>/dev/null)
    ifeq ($(BREW_PREFIX),)
        $(error "Homebrew not found. Please install Homebrew or GTest: brew install googletest")
    endif

    GTEST_INC := -I$(BREW_PREFIX)/include
    GTEST_LIB := -L$(BREW_PREFIX)/lib -lgtest -lgtest_main -pthread
endif

# Добавляем GTEST_INC в CXXFLAGS
CXXFLAGS += $(GTEST_INC)

# === Цели ===
.PHONY: all test clean s21_matrix_oop.a

all: $(LIB)

# Сборка библиотеки
$(LIB): $(OBJECTS)
	@mkdir -p $(dir $@)
	ar rcs $@ $^
	@echo "✅ Static library created: $(LIB)"

# Компиляция объектных файлов
$(BUILD_DIR)/%.o: ./%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "   [COMPILE] $<"

# Тесты
test: $(LIB) $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/test $(TEST_OBJECTS) $(LIB) $(GTEST_LIB)
	@echo "🧪 Running unit tests..."
	$(BUILD_DIR)/test

$(BUILD_DIR)/tests/%.o: ./tests/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "   [TEST] $<"

s21_matrix_oop.a: $(LIB)

clean:
	rm -rf $(BUILD_DIR) $(LIB)
	@echo "🧹 Clean completed"

.PHONY: test all $(LIB) memcheck cppcheck

# === Дополнительные цели ===

.PHONY: memcheck cppcheck clang-tidy style

# Проверка утечек памяти
memcheck: test
	@echo "🔍 Running Valgrind memory check..."
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 ./build/test

# Статический анализ: cppcheck
cppcheck:
	@echo "🔍 Running cppcheck..."
	cppcheck \
		--enable=warning,performance,portability,style,information \
		--inconclusive \
		--std=c++17 \
		-I. \
		--suppress=missingIncludeSystem \
		--quiet \
		--error-exitcode=1 \
		$(wildcard *.cpp tests/*.cpp)

# Статический анализ: clang-tidy
clang-tidy:
	@echo "🔍 Running clang-tidy..."
	clang-tidy \
		$(wildcard *.cpp) \
		-header-filter='.*' \
		-- -I. -std=c++17

# Проверка стиля (если установлен cpplint)
.PHONY: format check-format

# Форматировать весь код
format:
	@echo "✨ Formatting code with clang-format..."
	clang-format -i *.cpp *.h tests/*.cpp

# Проверить, отформатирован ли код (для CI)
check-format:
	@echo "🔍 Checking code style with clang-format..."
	@echo "If output is empty — code is well-formatted."
	clang-format --dry-run *.cpp *.h tests/*.cpp

# Дополнительно: показать различия
diff-format:
	@echo "📊 Showing diff after formatting..."
	clang-format -output-replacements-xml *.cpp *.h tests/*.cpp | grep "<replacement "

.PHONY: check

check: test memcheck cppcheck clang-tidy check-format
	@echo "🎉 All checks passed! Code is clean, safe, and stylish."