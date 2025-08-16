Отлично, понял задачу!

Теперь мы **перепишем методическое пособие с нуля**, начиная с **Этапа 2**, но с новым подходом:

---

## 📘 **Методическое пособие: От С к C++ на примере матрицы**  
**Для тех, кто знает С, но новичок в C++**  
*Уровень: от C к C++ (с нуля)*  
*Платформа: Linux (Ubuntu/Debian)*

---

## 🧱 Этап 2: Создание заголовочного файла — переход от `struct` к `class`

> 💡 Ты знаешь С: работал с `struct`, `malloc`, `#define`.  
> В C++ есть **классы**, которые — как `struct`, но **могут содержать функции (методы)**, **инкапсулировать данные** и **управлять памятью автоматически**.

---

### 🔧 2.1. Отличие `struct` в С от `class` в C++

| В С | В C++ |
|-----|-------|
| `struct Point { int x, y; };` | `class Point { int x, y; };` |
| Только данные | Данные + функции |
| Нет методов | Есть методы (функции внутри класса) |
| Поля публичные | По умолчанию — приватные |

> ✅ В C++ `class` — это как `struct`, но **с приватными полями по умолчанию** и возможностью добавлять **методы**.

---

### 🔧 2.2. Создаём заголовочный файл

Создай файл:

```bash
cd ~/cpp_matrix_project/src
touch s21_matrix_oop.h
```

Открой его и вставь:

```cpp
// src/s21_matrix_oop.h
#ifndef S21_MATRIX_OOP_H
#define S21_MATRIX_OOP_H

#include <stdexcept>  // для std::out_of_range

namespace s21 {  // Пространство имён — как "папка" для классов

class S21Matrix {
 public:
  // Конструкторы — как "функции создания"
  S21Matrix();                    // S21Matrix mat;
  S21Matrix(int rows, int cols);  // S21Matrix mat(2, 3);
  S21Matrix(const S21Matrix& other);           // Копирование
  S21Matrix(S21Matrix&& other) noexcept;       // Перемещение
  ~S21Matrix();                               // Деструктор — освобождает память

  // Геттеры (accessors)
  int get_rows() const;
  int get_cols() const;

  // Сеттеры (mutators)
  void set_rows(int rows);
  void set_cols(int cols);

  // Основные операции
  bool EqMatrix(const S21Matrix& other) const;
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(const double num);
  void MulMatrix(const S21Matrix& other);

  // Матричные операции
  S21Matrix Transpose() const;
  S21Matrix CalcComplements() const;
  double Determinant() const;
  S21Matrix InverseMatrix() const;

  // Перегрузка операторов — делаем матрицу "удобной"
  S21Matrix& operator=(const S21Matrix& other);
  S21Matrix& operator=(S21Matrix&& other) noexcept;
  S21Matrix operator+(const S21Matrix& other) const;
  S21Matrix operator-(const S21Matrix& other) const;
  S21Matrix operator*(const S21Matrix& other) const;
  S21Matrix operator*(const double num) const;
  bool operator==(const S21Matrix& other) const;
  S21Matrix& operator+=(const S21Matrix& other);
  S21Matrix& operator-=(const S21Matrix& other);
  S21Matrix& operator*=(const S21Matrix& other);
  S21Matrix& operator*=(const double num);
  double& operator()(int i, int j);           // mat(0, 0) = 5;
  const double& operator()(int i, int j) const;

 private:
  double** matrix_;  // Указатель на массив указателей — как в С
  int rows_;         // Количество строк
  int cols_;         // Количество столбцов

  // Вспомогательные методы (не для пользователя)
  void AllocateMatrix();     // Выделение памяти (как malloc в С)
  void FreeMatrix();         // Освобождение (как free)
  void CopyMatrix(const S21Matrix& other);  // Копирование данных
  double CalcDeterminant() const;
  S21Matrix GetMinorMatrix(int row, int col) const;
};

// Умножение числа на матрицу: 2.0 * matrix
S21Matrix operator*(const double num, const S21Matrix& matrix);

}  // namespace s21

#endif  // S21_MATRIX_OOP_H
```

---

### ✅ 2.3. Объяснение для программиста на С

| С | C++ |
|---|-----|
| `struct { int x, y; } point;` | `class Point { int x, y; };` |
| `point.x = 5;` | `point.get_x()` — лучше инкапсуляция |
| `malloc(rows * cols * sizeof(double))` | `new double*[rows]` — C++-выделение |
| `free(matrix)` | `~S21Matrix()` — деструктор вызывается **автоматически** |
| `matrix[i][j]` | `matrix(i, j)` — перегрузка `()` для удобства |
| `#define MAX 100` | `const int max = 100;` — безопаснее |

> 💡 В C++:
> - **Класс управляет своей памятью** — не нужно вручную вызывать `free`.
> - **Конструктор** — как `init()`, но вызывается автоматически.
> - **Деструктор** — как `cleanup()`, вызывается, когда объект уничтожается.

---

### 🔍 Пример: Что такое конструктор?

В С ты делал так:

```c
Matrix* create_matrix(int rows, int cols) {
    Matrix* m = malloc(sizeof(Matrix));
    m->rows = rows;
    m->cols = cols;
    m->data = malloc(rows * cols * sizeof(double));
    return m;
}
```

В C++ это делает **конструктор**:

```cpp
S21Matrix mat(3, 3);  // Автоматически вызывается конструктор
```

---

### ✅ 2.4. Зачем нужно `namespace s21`?

Как в С ты используешь префиксы:

```c
s21_matrix_create();
s21_matrix_free();
```

В C++ можно использовать **пространство имён**:

```cpp
namespace s21 {
    class S21Matrix { ... };
}
```

Теперь:
```cpp
s21::S21Matrix mat;  // как s21_matrix_create()
```

> ✅ Избегаем конфликтов имён, как в С с префиксами.

---

### ✅ 2.5. Почему `private`?

В С ты писал:

```c
// matrix.h
typedef struct {
    double* data;
    int rows, cols;
} Matrix;

// matrix.c
// Реализация — скрыта
```

В C++ можно **явно скрыть** данные:

```cpp
private:
  double** matrix_;
  int rows_, cols_;
```

> ✅ Пользователь не может напрямую менять `matrix_` — только через методы.

---

### 📚 Что изучить новичку:

| Тема | Объяснение |
|------|-----------|
| `class` vs `struct` | В C++ `class` — как `struct`, но поля по умолчанию `private` |
| Конструктор | Функция с именем класса — вызывается при создании |
| Деструктор | `~ClassName()` — вызывается при уничтожении |
| `new` / `delete` | Аналог `malloc` / `free`, но с вызовом конструкторов |
| `namespace` | Как префикс в С, но читаемее |
| `const` после метода | Гарантирует, что метод не меняет объект |

---

### ✅ Проверка

Убедись, что файл создан:

```bash
cat s21_matrix_oop.h
```

---

✅ **Этап 2 завершён.**  
Теперь ты понимаешь:
- Как **перейти от С к C++**,
- Что такое **класс, конструктор, деструктор**,
- Как **инкапсулировать данные**,
- И зачем нужен `namespace`.

---

Готов к **Этапу 3: Реализация конструкторов и управления памятью (как в С, но в C++)**?

Напиши: **"Следующий этап"** — и я продолжу.