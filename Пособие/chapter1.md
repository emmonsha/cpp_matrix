Отлично! 🎉  
Рад, что всё заработало.  
Теперь начнём создавать **методическое пособие** — пошаговое руководство, как собрать и понять этот проект, начиная с подготовки окружения.

---

## 📘 **Методическое пособие: Разработка C++ библиотеки с тестами и анализом кода**  
**Для Linux (Ubuntu/Debian)**  
*Уровень: от начального к продвинутому*

---

## 🧱 Этап 1: Подготовка окружения (Linux)

> ✅ Цель: Настроить систему для разработки C++ проектов с использованием компилятора, сборщика, тестирования и анализа кода.

---

### 🔧 1.1. Обновление системы

Открой терминал и выполните:

```bash
sudo apt update
sudo apt upgrade -y
```

> Это обеспечит установку последних версий пакетов.

---

### 🔧 1.2. Установка компилятора и сборщика

```bash
sudo apt install build-essential -y
```

> Устанавливает:
> - `g++` — компилятор C++
> - `gcc` — компилятор C
> - `make` — сборщик проектов
> - `libc` и другие системные библиотеки

Проверка:
```bash
g++ --version
make --version
```

---

### 🔧 1.3. Установка Git (опционально, но рекомендуется)

```bash
sudo apt install git -y
```

Настройка (если первый раз):
```bash
git config --global user.name "Твоё Имя"
git config --global user.email "твой@email.com"
```

---

### 🔧 1.4. Установка GTest (библиотека для unit-тестов)

```bash
sudo apt install libgtest-dev -y
```

> ⚠️ Устанавливает **исходники и заголовки** GTest.  
> Библиотека будет скомпилирована при сборке проекта.

Проверка:
```bash
ls /usr/src/googletest        # исходники
ls /usr/include/gtest         # заголовки
```

---

### 🔧 1.5. Установка инструментов анализа

#### ✅ Valgrind — проверка утечек памяти
```bash
sudo apt install valgrind -y
```

Проверка:
```bash
valgrind --version
```

#### ✅ Cppcheck — статический анализ
```bash
sudo apt install cppcheck -y
```

Проверка:
```bash
cppcheck --version
```

#### ✅ Clang-Format — автоформатирование
```bash
sudo apt install clang-format -y
```

Проверка:
```bash
clang-format --version
```

---

### 🔧 1.6. Установка Python и pipx (для утилит)

```bash
sudo apt install python3 python3-pip -y
pip3 install --user pipx
pipx ensurepath
```

> Это нужно, если будешь использовать другие Python-инструменты в будущем.

---

### 🔧 1.7. Создание структуры проекта

Создай папку проекта:

```bash
mkdir ~/cpp_matrix_project
cd ~/cpp_matrix_project
mkdir src
```

Теперь все файлы будут в `src/`.

---

### ✅ Итог: твоё окружение готово к разработке

| Инструмент | Назначение |
|----------|-----------|
| `g++` | Компиляция C++ |
| `make` | Сборка проекта |
| `libgtest-dev` | Unit-тесты |
| `valgrind` | Проверка утечек памяти |
| `cppcheck` | Статический анализ |
| `clang-format` | Форматирование кода |

---

### 📚 Что изучить дополнительно:

| Тема | Ресурс |
|------|--------|
| Основы C++ | [learncpp.com](https://www.learncpp.com/) |
| Google C++ Style Guide | [google.github.io/styleguide/cppguide.html](https://google.github.io/styleguide/cppguide.html) |
| Makefile | [GNU Make Manual](https://www.gnu.org/software/make/manual/make.html) |
| GTest | [Google Test Primer](https://github.com/google/googletest/blob/main/docs/primer.md) |
| Valgrind | [Valgrind Quick Start](https://valgrind.org/docs/manual/quick-start.html) |
| Clang-Format | [Clang-Format Docs](https://clang.llvm.org/docs/ClangFormat.html) |

---

✅ **Этап 1 завершён.**  
Теперь система готова к разработке.

Готов к **Этапу 2: Создание структуры проекта и написание заголовочного файла**?

Напиши: **"Следующий этап"** — и я продолжу.