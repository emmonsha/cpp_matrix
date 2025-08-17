
---

## ✅ Добавляем:

1. `.gitignore` — чтобы не коммитить мусор
2. `CMakeLists.txt` — современная альтернатива `Makefile`
3. `.github/workflows/ci.yml` — GitHub Actions CI/CD

---

## 📁 Структура проекта (финальная)

```
cpp_matrix_project/
├── src/
│   ├── s21_matrix_oop.h
│   ├── constructors.cpp
│   ├── destructors.cpp
│   ├── accessors.cpp
│   ├── arithmetic_ops.cpp
│   ├── matrix_ops.cpp
│   ├── operators.cpp
│   ├── tests/
│   │   ├── main_test.cpp
│   │   ├── constructors_test.cpp
│   │   └── ...
│   ├── Makefile
│   ├── Doxyfile
│   └── CMakeLists.txt
├── .gitignore
├── README.md
└── .github/
    └── workflows/
        └── ci.yml
```

---

## 1. `.gitignore`

Создай в корне проекта файл `.gitignore`:

```gitignore
# Build directories
/build/
/lib/
/docs/html/
cmake-build-*/

# CMake
CMakeCache.txt
CMakeFiles/
cmake_install.cmake
Makefile
install_manifest.txt
CTestTestfile.cmake

# Compiled binaries
test
a.out
*.o
*.a
*.so
*.dylib
*.dll

# IDE
.vscode/
*.swp
*.swo
.DS_Store
*.sublime-project
*.sublime-workspace

# Logs
*.log
valgrind.log

# Temporary files
*~
*.tmp
```

---

## 2. `src/CMakeLists.txt`

Создай файл `src/CMakeLists.txt`:

```cmake
# src/CMakeLists.txt
cmake_minimum_required(VERSION 3.14)
project(S21Matrix LANGUAGES CXX)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

# Source files
set(SOURCES
    constructors.cpp
    destructors.cpp
    accessors.cpp
    arithmetic_ops.cpp
    matrix_ops.cpp
    operators.cpp
)

# Library
add_library(s21matrix STATIC ${SOURCES})

# Headers
target_include_directories(s21matrix PUBLIC .)

# Find GTest
find_package(GTest REQUIRED)

# Test executable
enable_testing()
add_executable(tests 
    tests/main_test.cpp
    tests/constructors_test.cpp
    tests/accessors_test.cpp
    tests/arithmetic_ops_test.cpp
    tests/matrix_ops_test.cpp
    tests/operators_test.cpp
)

# Link
target_link_libraries(tests s21matrix GTest::gtest GTest::gtest_main)

# Clang-Format target (optional)
find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
        COMMENT "Formatting code with clang-format"
    )
endif()

# Doxygen (optional)
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/docs/html)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
        COMMENT "Generating documentation with Doxygen"
        VERBATIM
    )
    add_custom_command(TARGET docs POST_BUILD
        COMMAND echo "Documentation generated in ${DOXYGEN_OUT}"
    )
endif()
```

---

## 3. `.github/workflows/ci.yml`

Создай папку и файл:

```bash
mkdir -p .github/workflows
touch .github/workflows/ci.yml
```

Содержимое `ci.yml`:

```yaml
name: C++ CI

on:
  push:
    branches: [ main, master ]
  pull_request:
    branches: [ main, master ]

jobs:
  build-and-test:
    runs-on: ubuntu-latest

    steps:
    - name: Checkout code
      uses: actions/checkout@v4

    - name: Install dependencies
      run: |
        sudo apt-get update
        sudo apt-get install -y g++ make cmake valgrind cppcheck clang-format doxygen graphviz libgtest-dev

    - name: Build with CMake
      run: |
        cd src
        mkdir build && cd build
        cmake ..
        make

    - name: Run unit tests
      run: |
        cd src/build
        ctest --output-on-failure

    - name: Check memory leaks with Valgrind
      run: |
        cd src/build
        valgrind --error-exitcode=1 --leak-check=full ./tests

    - name: Check code style
      run: |
        cd src
        clang-format --dry-run *.cpp *.h tests/*.cpp

    - name: Build documentation
      run: |
        cd src
        doxygen Doxyfile
```

---

## ✅ Как использовать CMake

### Локальная сборка:

```bash
cd src
mkdir build
cd build
cmake ..
make
./tests
```

### Или через `make` (если хочешь оставить Makefile)

Теперь у тебя **два способа сборки**:
- `make test` — через Makefile
- `cmake` — современный способ

---

## ✅ Что даёт GitHub Actions

При каждом `git push`:
- ✅ Автоматически собирает проект
- ✅ Запускает тесты
- ✅ Проверяет утечки памяти
- ✅ Проверяет стиль
- ✅ Генерирует документацию

---

## ✅ Финальные команды

```bash
# В корне проекта
git init
git add .
git commit -m "Initial commit"
git remote add origin https://github.com/ваше_имя/s21matrix.git
git branch -M main
git push -u origin main
```

---

## 🎉 Готово!

Теперь у тебя **полноценный профессиональный C++ проект** с:

| Функция | Инструмент |
|-------|-----------|
| Управление сборкой | `Makefile` + `CMakeLists.txt` |
| Тестирование | `Google Test` |
| Анализ памяти | `Valgrind` |
| Статический анализ | `cppcheck` |
| Форматирование | `clang-format` |
| Документация | `Doxygen` |
| CI/CD | `GitHub Actions` |
| Версионность | `.gitignore`, `README.md` |

---

- [Назад к оглавлению](README.md)
- [Покрытие кода: `gcov` + `lcov`, CodeQL — анализ безопасности,  Шаблон релиза](12:gcov_lcov_release.md)