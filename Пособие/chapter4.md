Отлично! Переходим к **Этапу 4** — реализации **accessors, mutators и базовых операций**, но с акцентом на **переход от С к C++**.

---

## 📘 Этап 4: Геттеры, сеттеры и арифметические операции — как в С, но удобнее

> 💡 В С ты писал:
> ```c
> get_rows(matrix);
> add_matrices(a, b, result);
> ```
>
> В C++ можно писать:
> ```cpp
> matrix.get_rows();
> matrix = a + b;
> ```
>
> Это — **инкапсуляция + перегрузка операторов** = удобство и читаемость.

---

### 🔧 4.1. Создаём файл `accessors.cpp`

```bash
touch accessors.cpp
```

Открой его:

```cpp
// src/accessors.cpp
#include "s21_matrix_oop.h"
#include <algorithm>  // для std::min, std::copy

namespace s21 {

// === Accessors (геттеры) ===

int S21Matrix::get_rows() const {
  return rows_;
}

int S21Matrix::get_cols() const {
  return cols_;
}

// === Mutators (сеттеры) ===

// Изменение количества строк
void S21Matrix::set_rows(int rows) {
  if (rows <= 0) {
    throw std::out_of_range("Rows must be positive.");
  }

  // Если размер не меняется — выходим
  if (rows == rows_) return;

  // Создаём новую матрицу с новым числом строк
  double** new_matrix = new double*[rows];

  // Выделяем память для каждой строки
  for (int i = 0; i < rows; ++i) {
    new_matrix[i] = new double[cols_]();  // () — обнуляем
  }

  // Копируем данные из старой матрицы
  int copy_rows = std::min(rows, rows_);
  for (int i = 0; i < copy_rows; ++i) {
    std::copy(matrix_[i], matrix_[i] + cols_, new_matrix[i]);
  }

  // Освобождаем старую память
  FreeMatrix();

  // Подключаем новую
  matrix_ = new_matrix;
  rows_ = rows;
}

// Изменение количества столбцов
void S21Matrix::set_cols(int cols) {
  if (cols <= 0) {
    throw std::out_of_range("Cols must be positive.");
  }

  if (cols == cols_) return;

  double** new_matrix = new double*[rows_];
  for (int i = 0; i < rows_; ++i) {
    new_matrix[i] = new double[cols]();
    // Копируем минимум между старым и новым размером
    std::copy(matrix_[i], matrix_[i] + std::min(cols, cols_), new_matrix[i]);
  }

  FreeMatrix();
  matrix_ = new_matrix;
  cols_ = cols;
}

}  // namespace s21
```

---

### ✅ 4.2. Сравнение: С vs C++

| Задача | В С | В C++ |
|-------|-----|--------|
| Получить строки | `int rows = matrix->rows;` | `int rows = matrix.get_rows();` |
| Изменить строки | `resize_matrix_rows(matrix, 5);` | `matrix.set_rows(5);` |
| Проверка ошибок | `if (rows <= 0) return ERROR;` | `if (rows <= 0) throw std::out_of_range;` |
| Копирование данных | `memcpy` или цикл | `std::copy` — безопаснее |
| Увеличение матрицы | Вручную | Автоматически с заполнением нулями |

> ✅ В C++:
> - **Инкапсуляция** — пользователь не видит `rows_`, только `get_rows()`
> - **Контроль доступа** — нельзя случайно сломать объект
> - **Исключения** — лучше, чем коды ошибок

---

### 🔧 4.3. Файл `arithmetic_ops.cpp` — арифметика как в С, но в C++ стиле

```bash
touch arithmetic_ops.cpp
```

```cpp
// src/arithmetic_ops.cpp
#include "s21_matrix_oop.h"
#include <stdexcept>

namespace s21 {

// === Сравнение матриц ===
bool S21Matrix::EqMatrix(const S21Matrix& other) const {
  constexpr double kEpsilon = 1e-7;  // для сравнения double

  if (rows_ != other.rows_ || cols_ != other.cols_) return false;

  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      if (std::abs(matrix_[i][j] - other.matrix_[i][j]) > kEpsilon) {
        return false;
      }
    }
  }
  return true;
}

// === Сложение ===
void S21Matrix::SumMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::out_of_range("Matrix dimensions must match.");
  }
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] += other.matrix_[i][j];
    }
  }
}

// === Вычитание ===
void S21Matrix::SubMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::out_of_range("Matrix dimensions must match.");
  }
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] -= other.matrix_[i][j];
    }
  }
}

// === Умножение на число ===
void S21Matrix::MulNumber(const double num) {
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] *= num;
    }
  }
}

// === Умножение матриц ===
void S21Matrix::MulMatrix(const S21Matrix& other) {
  if (cols_ != other.rows_) {
    throw std::out_of_range("Invalid matrix dimensions for multiplication.");
  }

  // Создаём временную матрицу для результата
  S21Matrix result(rows_, other.cols_);

  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < other.cols_; ++j) {
      result.matrix_[i][j] = 0;
      for (int k = 0; k < cols_; ++k) {
        result.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
      }
    }
  }

  // Заменяем текущую матрицу на результат
  *this = std::move(result);
}

}  // namespace s21
```

---

### ✅ 4.4. Объяснение для новичка

| Концепция | Объяснение |
|----------|-----------|
| `get_rows()`, `set_rows()` | Как `getter/setter` — контролируют доступ к данным |
| `throw std::out_of_range` | Аналог `return -1;`, но с сообщением об ошибке |
| `std::abs`, `std::copy` | Готовые функции из стандартной библиотеки — не пиши сам |
| `std::move(result)` | "Забирает" данные у временного объекта — эффективно |
| `*this = ...` | Присваивание текущему объекту — как `assign_matrix(&a, &b)` в С |

---

### 📚 Что изучить дополнительно:

| Тема | Почему важно |
|------|-------------|
| [std::copy](https://en.cppreference.com/w/cpp/algorithm/copy) | Безопасная замена циклов |
| [Исключения](https://en.cppreference.com/w/cpp/error/exception) | Лучше, чем коды ошибок |
| [std::move](https://en.cppreference.com/w/cpp/utility/move) | Эффективное перемещение объектов |
| [kEpsilon для double](https://floating-point-gui.de/) | Почему `==` для `double` — плохо |

---

### ✅ Проверка

Убедись, что файлы созданы:

```bash
ls -l accessors.cpp arithmetic_ops.cpp
```

---

✅ **Этап 4 завершён.**  
Теперь ты знаешь:
- Как **реализовать геттеры/сеттеры** с контролем ошибок.
- Как **перевести С-логику** в C++ с использованием STL.
- Что **исключения** — мощная альтернатива кодам ошибок.

---

Готов к **Этапу 5: Реализация матричных операций (транспонирование, определитель, обратная матрица)**?

Напиши: **"Следующий этап"** — и я продолжу.