
---

## 📘 Этап 5: Матричные операции — от математики к коду

> 💡 Ты знаешь математику:  
> - Транспонирование: \( A^T_{ij} = A_{ji} \)  
> - Определитель — через миноры  
> - Обратная матрица — через союзную матрицу и определитель  
>
> Теперь реализуем это **на C++**, с учётом ошибок и эффективности.

---

### 🔧 5.1. Создаём файл `matrix_ops.cpp`

```bash
touch matrix_ops.cpp
```

Открой его:

```cpp
// src/matrix_ops.cpp
#include "s21_matrix_oop.h"
#include <algorithm>
#include <cmath>

namespace s21 {

// === Транспонирование: rows x cols → cols x rows ===
S21Matrix S21Matrix::Transpose() const {
  // Новая матрица: размеры перевёрнуты
  S21Matrix result(cols_, rows_);

  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      result.matrix_[j][i] = matrix_[i][j];  // A[j][i] = A^T[i][j]
    }
  }

  return result;
}

// === Вспомогательный метод: получение минора ===
// Удаляем строку row и столбец col
S21Matrix S21Matrix::GetMinorMatrix(int row, int col) const {
  S21Matrix minor(rows_ - 1, cols_ - 1);

  int minor_i = 0;
  for (int i = 0; i < rows_; ++i) {
    if (i == row) continue;  // пропускаем строку
    int minor_j = 0;
    for (int j = 0; j < cols_; ++j) {
      if (j == col) continue;  // пропускаем столбец
      minor.matrix_[minor_i][minor_j] = matrix_[i][j];
      ++minor_j;
    }
    ++minor_i;
  }

  return minor;
}

// === Рекурсивное вычисление определителя ===
double S21Matrix::CalcDeterminant() const {
  // Базовые случаи
  if (rows_ == 1) {
    return matrix_[0][0];
  }
  if (rows_ == 2) {
    return matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
  }

  // Для матриц 3x3 и больше — разложение по первой строке
  double det = 0;
  for (int j = 0; j < cols_; ++j) {
    // Минор для элемента (0, j)
    S21Matrix minor = GetMinorMatrix(0, j);

    // Алгебраическое дополнение: (-1)^(i+j) * det(minor)
    double cofactor = minor.Determinant() * ((j % 2 == 0) ? 1 : -1);

    // det += a[0][j] * cofactor
    det += matrix_[0][j] * cofactor;
  }

  return det;
}

// === Определитель (публичный метод) ===
double S21Matrix::Determinant() const {
  if (rows_ != cols_) {
    throw std::out_of_range("Matrix must be square to calculate determinant.");
  }
  return CalcDeterminant();
}

// === Матрица алгебраических дополнений ===
S21Matrix S21Matrix::CalcComplements() const {
  if (rows_ != cols_) {
    throw std::out_of_range("Matrix must be square to calculate complements.");
  }

  if (rows_ == 1) {
    S21Matrix result(1, 1);
    result.matrix_[0][0] = 1;
    return result;
  }

  S21Matrix result(rows_, cols_);

  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      S21Matrix minor = GetMinorMatrix(i, j);
      double det = minor.Determinant();
      // Знак: (-1)^(i+j)
      result.matrix_[i][j] = det * ((i + j) % 2 == 0 ? 1 : -1);
    }
  }

  return result;
}

// === Обратная матрица: A^(-1) = (1/det) * C^T ===
S21Matrix S21Matrix::InverseMatrix() const {
  double det = Determinant();
  constexpr double kEpsilon = 1e-7;

  if (std::abs(det) < kEpsilon) {
    throw std::out_of_range("Matrix is singular (determinant is zero).");
  }

  // 1. Матрица алгебраических дополнений
  S21Matrix complements = CalcComplements();

  // 2. Транспонируем её (получаем союзную матрицу)
  S21Matrix adjugate = complements.Transpose();

  // 3. Делим на определитель
  adjugate.MulNumber(1.0 / det);

  return adjugate;
}

}  // namespace s21
```

---

### ✅ 5.2. Объяснение для новичка

| Операция | Математика | Как в коде |
|--------|-----------|-----------|
| **Транспонирование** | \( A^T_{ij} = A_{ji} \) | Цикл: `result[j][i] = matrix[i][j]` |
| **Минор** | Удалить строку и столбец | `GetMinorMatrix(i, j)` — копируем все, кроме i-строки и j-столбца |
| **Определитель** | Через миноры (рекурсия) | `CalcDeterminant()` вызывает `Determinant()` для миноров |
| **Алгебраическое дополнение** | \( C_{ij} = (-1)^{i+j} \cdot \det(M_{ij}) \) | Считаем минор, умножаем на знак |
| **Обратная матрица** | \( A^{-1} = \frac{1}{\det A} C^T \) | `CalcComplements() → Transpose() → MulNumber(1/det)` |

---

### 🔍 Пример: Почему `std::abs(det) < 1e-7`?

В С ты писал:
```c
if (det == 0) ...
```

Но `double` — неточный тип.  
В C++ лучше:
```cpp
if (std::abs(det) < 1e-7)  // "почти ноль"
```

> ✅ Это стандартный способ сравнения `double`.

---

### ✅ 5.3. Обработка ошибок

| Ошибка | Как обрабатываем |
|-------|------------------|
| Не квадратная матрица | `throw std::out_of_range("Matrix must be square")` |
| Определитель = 0 | `throw std::out_of_range("Matrix is singular")` |
| Некорректные размеры | Проверяем в начале метода |

---

### 📚 Что изучить дополнительно:

| Тема | Ресурс |
|------|--------|
| [Рекурсия](https://en.cppreference.com/w/cpp/language/recursion) | `Determinant()` вызывает себя через миноры |
| [Транспонирование](https://ru.wikipedia.org/wiki/Транспонированная_матрица) | Просто поменять индексы |
| [Обратная матрица](https://ru.wikipedia.org/wiki/Обратная_матрица) | Через союзную матрицу |
| [std::abs](https://en.cppreference.com/w/cpp/numeric/math/fabs) | Для `double` — `std::abs`, не `fabs` |

---

### ✅ Проверка

Убедись, что файл создан:

```bash
ls -l matrix_ops.cpp
```

---

✅ **Этап 5 завершён.**  
Теперь ты умеешь:
- Реализовывать **математические операции** на C++.
- Работать с **рекурсией** (определитель).
- Обрабатывать **исключительные ситуации**.
- Писать **чистый, понятный код**, близкий к математике.

---

- [Назад к оглавлению](README.md)
- [📘 Этап 6: Перегрузка операторов — как сделать матрицу удобной, как в математике](chapter6.md)