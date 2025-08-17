
1. 🌐 **GitHub Pages** — публикация документации
2. 🧪 **Codecov.io** — отображение % покрытия в PR
3. 🐳 **Dockerfile** — воспроизводимость окружения
4. 🌍 **Мультиплатформенность** — сборка на Linux, macOS, Windows

---

## 🚀 Этап 1: GitHub Pages — публикация документации

### 🔧 1.1. Включить GitHub Pages

1. Перейди в репозиторий на GitHub → `Settings` → `Pages`
2. Source: **Deploy from a branch**
3. Branch: `main` / `master`, Folder: `/docs`
4. Нажми **Save**

> ✅ Через 1–2 минуты будет доступно:  
> `https://<ваше_имя>.github.io/<название_репозитория>/`

---

### 🔧 1.2. Автоматическое обновление документации

Обнови `.github/workflows/ci.yml`:

```yaml
    - name: Deploy Docs to GitHub Pages
      if: ${{ github.ref == 'refs/heads/main' }}
      run: |
        mkdir -p docs
        cp -r src/docs/html/* docs/
        cd docs
        echo "s21matrix.github.io" > CNAME  # замени на своё
      continue-on-error: true

    - name: Commit & Push Docs
      if: ${{ github.ref == 'refs/heads/main' }}
      run: |
        git config user.name "GitHub Actions"
        git config user.email "actions@github.com"
        git add docs/
        git commit -m "📚 Update docs" || echo "No changes to commit"
        git push origin main
```

> Теперь `docs/html/` → копируется в корень → GitHub Pages публикует.

---

## 📊 Этап 2: Codecov.io — отображение покрытия

### 🔧 2.1. Зарегистрируйся на [codecov.io](https://codecov.io)

1. Войди через GitHub
2. Добавь свой репозиторий

### 🔧 2.2. Обнови `.github/workflows/ci.yml`

Добавь после `make coverage`:

```yaml
    - name: Upload coverage to Codecov
      uses: codecov/codecov-action@v4
      with:
        file: ./src/build-coverage/coverage.info
        flags: unittests
        name: ${{ github.job }}
        fail_ci_if_error: false
```

---

### 🔧 2.3. Добавь бейдж в `README.md`

В начало `README.md`:

```markdown
![Coverage](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/<ваш_gist>/raw/coverage.json)
```

> Или используй:
```markdown
![Codecov](https://img.shields.io/codecov/c/github/<ваше_имя>/<репозиторий>)
```

---

## 🐳 Этап 3: Dockerfile — воспроизводимость

Создай `Dockerfile` в корне:

```Dockerfile
# Dockerfile
FROM ubuntu:22.04

LABEL maintainer="you@example.com"
LABEL description="S21Matrix C++ Development Environment"

# Установка зависимостей
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    g++ \
    git \
    valgrind \
    cppcheck \
    clang-format \
    doxygen \
    graphviz \
    lcov \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

# Установка cpplint
RUN pip3 install cpplint

# Рабочая директория
WORKDIR /app

# Копируем только src (без build)
COPY src/ src/

# Сборка и тесты (опционально)
# RUN cd src && make test

# Порт для отладки (если нужно)
EXPOSE 8080

# Команда по умолчанию
CMD ["bash"]
```

---

### 🔧 3.1. Как использовать

```bash
# Собрать образ
docker build -t s21matrix-dev .

# Запустить контейнер
docker run -it -v $(pwd)/src:/app/src s21matrix-dev

# В контейнере
cd src
make test
make coverage
```

---

## 🌍 Этап 4: Мультиплатформенность

Обновим `.github/workflows/ci.yml`, чтобы тестировать на **Linux, macOS, Windows**.

### 🔧 4.1. Обновлённый `ci.yml`

```yaml
name: CI

on: [push, pull_request]

jobs:
  build:
    name: ${{ matrix.os }} - ${{ matrix.config }}
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [ubuntu-latest, windows-latest, macos-latest]
        config: [cmake, make]

    steps:
    - name: Checkout
      uses: actions/checkout@v4

    - name: Setup (Linux/macOS)
      if: ${{ runner.os != 'Windows' }}
      run: |
        sudo apt-get update
        sudo apt-get install -y g++ make cmake valgrind cppcheck clang-format lcov doxygen graphviz libgtest-dev

    - name: Setup (Windows)
      if: ${{ runner.os == 'Windows' }}
      run: |
        choco install mingw cmake ninja python git
        python -m pip install cpplint

    - name: Build with CMake
      if: ${{ matrix.config == 'cmake' }}
      run: |
        cd src
        mkdir build && cd build
        cmake ..
        cmake --build .
        ctest --output-on-failure

    - name: Build with Make
      if: ${{ matrix.config == 'make' && runner.os != 'Windows' }}
      run: |
        cd src
        make test

    - name: Valgrind (Linux only)
      if: ${{ runner.os == 'Linux' && matrix.config == 'cmake' }}
      run: |
        cd src/build
        valgrind --error-exitcode=1 --leak-check=full ./tests

    - name: Code Coverage (Linux)
      if: ${{ runner.os == 'Linux' && matrix.config == 'cmake' }}
      run: |
        cd src
        mkdir build-coverage && cd build-coverage
        cmake .. -DENABLE_COVERAGE=ON
        make
        ./tests
        make coverage
        bash <(curl -s https://codecov.io/bash) -f coverage.info

    - name: Format Check
      run: |
        cd src
        clang-format --dry-run *.cpp *.h tests/*.cpp
```

---

## ✅ Итог: твой проект теперь

| Функция | Готово? | Инструмент |
|-------|--------|----------|
| GitHub Pages | ✅ | Авто-деплой `docs/` |
| Codecov.io | ✅ | Покрытие в PR |
| Docker | ✅ | Воспроизводимость |
| Мультиплатформа | ✅ | Linux, macOS, Windows |
| CI/CD | ✅ | Автотесты, стиль, безопасность |
| Документация | ✅ | Doxygen + HTML |
| Релизы | ✅ | Шаблон |

---

## 🎉 Поздравляю!

Ты создал **полноценный open-source C++ проект уровня профессиональной разработки**.

---

## 📦 Что дальше?

- ✅ Опубликуй на **GitHub**
- ✅ Добавь **бейджи** в `README.md`:
  ```markdown
  ![CI](https://github.com/ваше_имя/репозиторий/actions/workflows/ci.yml/badge.svg)
  ![Codecov](https://img.shields.io/codecov/c/github/ваше_имя/репозиторий)
  ![License](https://img.shields.io/badge/license-MIT-blue)
  ```
- ✅ Пригласи других разработчиков
- ✅ Добавь `CONTRIBUTING.md`

---

- [Назад к оглавлению](README.md)
- [LICENSE (MIT), CONTRIBUTING.md,  CODE_OF_CONDUCT.md, SECURITY.md, .github/dependabot.yml, Обновлённый README.md, .editorconfig](/Пособие/14)