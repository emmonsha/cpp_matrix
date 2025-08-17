
---

## 📘 Этап 6: Перегрузка операторов — как сделать матрицу удобной, как в математике

> 💡 В С ты писал:
> ```c
> add_matrices(a, b, result);
> multiply_matrix_by_scalar(m, 2.0, result);
> ```
>
> В C++ можно писать:
> ```cpp
> result = a + b;
> result = m * 2.0;
> ```
>
> Это — **перегрузка операторов**: делаем код **читаемым и естественным**, как в математике.

---

### 🔧 6.1. Создаём файл `operators.cpp`

```bash
touch operators.cpp
```

Открой его:

```cpp
// src/operators.cpp
#include "s21_matrix_oop.h"

namespace s21 {

// === Присваивание ===

// Копирующее присваивание: a = b;
S21Matrix& S21Matrix::operator=(const S21Matrix& other) {
  // Защита от самоприсваивания: a = a;
  if (this != &other) {
    FreeMatrix();           // Освобождаем старую память
    rows_ = other.rows_;
    cols_ = other.cols_;
    AllocateMatrix();       // Выделяем новую
    CopyMatrix(other);      // Копируем данные
  }
  return *this;  // Позволяет писать: a = b = c;
}

// Перемещающее присваивание: a = std::move(b);
S21Matrix& S21Matrix::operator=(S21Matrix&& other) noexcept {
  if (this != &other) {
    FreeMatrix();               // Освобождаем свою память
    matrix_ = other.matrix_;    // "Забираем" ресурсы
    rows_ = other.rows_;
    cols_ = other.cols_;
    other.matrix_ = nullptr;    // Обнуляем исходный объект
    other.rows_ = 0;
    other.cols_ = 0;
  }
  return *this;
}

// === Арифметические операторы (возвращают новую матрицу) ===

// Сложение: c = a + b;
S21Matrix S21Matrix::operator+(const S21Matrix& other) const {
  S21Matrix result(*this);   // Копия текущей матрицы
  result.SumMatrix(other);   // Выполняем сложение
  return result;             // Возвращаем результат
}

// Вычитание: c = a - b;
S21Matrix S21Matrix::operator-(const S21Matrix& other) const {
  S21Matrix result(*this);
  result.SubMatrix(other);
  return result;
}

// Умножение матриц: c = a * b;
S21Matrix S21Matrix::operator*(const S21Matrix& other) const {
  S21Matrix result(*this);
  result.MulMatrix(other);
  return result;
}

// Умножение на число: c = a * 2.0;
S21Matrix S21Matrix::operator*(const double num) const {
  S21Matrix result(*this);
  result.MulNumber(num);
  return result;
}

// === Сравнение ===

// Проверка на равенство: if (a == b)
bool S21Matrix::operator==(const S21Matrix& other) const {
  return EqMatrix(other);
}

// === Присваивающие операторы (a += b) ===

// a += b
S21Matrix& S21Matrix::operator+=(const S21Matrix& other) {
  SumMatrix(other);
  return *this;
}

// a -= b
S21Matrix& S21Matrix::operator-=(const S21Matrix& other) {
  SubMatrix(other);
  return *this;
}

// a *= matrix
S21Matrix& S21Matrix::operator*=(const S21Matrix& other) {
  MulMatrix(other);
  return *this;
}

// a *= 2.0
S21Matrix& S21Matrix::operator*=(const double num) {
  MulNumber(num);
  return *this;
}

// === Индексация: matrix(i, j) ===

// Для изменения элемента: matrix(0, 0) = 5;
double& S21Matrix::operator()(int i, int j) {
  if (i < 0 || i >= rows_ || j < 0 || j >= cols_) {
    throw std::out_of_range("Index out of bounds.");
  }
  return matrix_[i][j];
}

// Для чтения элемента: double x = matrix(0, 0);
const double& S21Matrix::operator()(int i, int j) const {
  if (i < 0 || i >= rows_ || j < 0 || j >= cols_) {
    throw std::out_of_range("Index out of bounds.");
  }
  return matrix_[i][j];
}

// === Умножение числа на матрицу: 2.0 * matrix ===
// Это — не метод класса, а отдельная функция
S21Matrix operator*(const double num, const S21Matrix& matrix) {
  return matrix * num;  // Используем уже реализованный operator*
}

}  // namespace s21
```

---

### ✅ 6.2. Объяснение для новичка

| Оператор | Что делает | Аналог в С |
|--------|-----------|-----------|
| `a + b` | Сложение | `add_matrices(a, b, &result)` |
| `a == b` | Сравнение | `eq_matrices(a, b)` |
| `a(0, 1)` | Доступ к элементу | `matrix->data[0][1]` |
| `a *= 2` | Умножение с присваиванием | `mul_matrix_by_scalar_inplace(a, 2)` |
| `2 * a` | Умножение числа на матрицу | Нужна **отдельная функция**, т.к. число — не объект |

> ✅ Перегрузка операторов — это **синтаксический сахар**, который делает код **естественнее**.

---

### 🔍 Почему `operator*` для `2 * matrix` — отдельная функция?

Потому что:
```cpp
2 * matrix
```
→ `2` — `double`, у него **нет метода `operator*`**, который принимает `S21Matrix`.

Поэтому нужно **глобальная функция**:
```cpp
S21Matrix operator*(const double num, const S21Matrix& matrix)
```

> ✅ Она определена **вне класса**, но в том же `namespace`.

---

### ✅ 6.3. Правила перегрузки операторов

| Правило | Пример |
|-------|--------|
| Не меняй смысл оператора | `+` — сложение, не вычитание |
| Возвращай `*this` для `+=`, `-=`, `*=` | Чтобы работало: `a += b += c;` |
| Возвращай копию для `+`, `-`, `*` | `a + b` — новая матрица |
| Проверяй границы | `operator()` — бросай исключение при выходе за пределы |
| Избегай самоприсваивания | `if (this != &other)` в `operator=` |

---

### 📚 Что изучить дополнительно:

| Тема | Почему важно |
|------|-------------|
| [Перегрузка операторов](https://en.cppreference.com/w/cpp/language/operators) | Делает классы удобными |
| [Friend functions](https://en.cppreference.com/w/cpp/language/friend) | Если нужно доступ к `private`, но не требуется |
| [Copy-and-swap idiom](https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom) | Альтернатива ручному `operator=` |
| [Operator overloading rules](https://isocpp.org/wiki/faq/operator-overloading) | Лучшие практики |

---

### ✅ Проверка

Убедись, что файл создан:

```bash
ls -l operators.cpp
```

---

✅ **Этап 6 завершён.**  
Теперь ты умеешь:
- Делать классы **удобными**, как встроенные типы.
- Перегружать **арифметические, присваивающие и индексные операторы**.
- Писать **естественный код**: `a = b + c * 2;`

---

- [Назад к оглавлению](README.md)
- [📘 Этап 7: Unit-тесты с Google Test — как проверить, что код работает](chapter7.md)