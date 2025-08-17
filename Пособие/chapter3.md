## 📘 Этап 3: Реализация конструкторов и управления памятью — как в С, но в стиле C++

> 💡 Ты знаешь `malloc`, `free`, `for`-циклы.  
> В C++ есть **аналоги**: `new`, `delete`, но с автоматическим управлением — и это **ключевое преимущество**.

---

### 🔧 3.1. Создаём файлы реализации

```bash
cd ~/cpp_matrix_project/src
touch constructors.cpp
touch destructors.cpp
```

---

### ✅ 3.2. Реализация в `constructors.cpp` — как `init()` в С, но лучше

Открой `constructors.cpp`:

```cpp
// src/constructors.cpp
#include "s21_matrix_oop.h"
#include <stdexcept>  // для std::out_of_range

namespace s21 {

// === Конструкторы — как функции создания матрицы ===

// 1. Базовый конструктор: создаёт 3x3 матрицу
// Работает как: S21Matrix mat;
S21Matrix::S21Matrix() : rows_(3), cols_(3) {
  AllocateMatrix();  // Выделяем память — как malloc в С
}

// 2. Параметризированный конструктор: S21Matrix mat(2, 3);
S21Matrix::S21Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
  if (rows <= 0 || cols <= 0) {
    throw std::out_of_range("Rows and cols must be positive.");
  }
  AllocateMatrix();  // Выделяем память
}

// 3. Конструктор копирования: S21Matrix mat2 = mat1;
// Работает как deep copy — как если бы ты писал copy_matrix() в С
S21Matrix::S21Matrix(const S21Matrix& other)
    : rows_(other.rows_), cols_(other.cols_) {
  AllocateMatrix();      // Выделяем новую память
  CopyMatrix(other);     // Копируем данные — как в С
}

// 4. Конструктор перемещения: S21Matrix mat2 = std::move(mat1);
// "Забирает" память у другого объекта — эффективно, без копирования
S21Matrix::S21Matrix(S21Matrix&& other) noexcept
    : matrix_(other.matrix_), rows_(other.rows_), cols_(other.cols_) {
  // Обнуляем исходный объект — как если бы ты обнулил указатель после free
  other.matrix_ = nullptr;
  other.rows_ = 0;
  other.cols_ = 0;
}

// === Вспомогательные методы — как функции в matrix.c ===

// AllocateMatrix() — как create_matrix_data() в С
void S21Matrix::AllocateMatrix() {
  // Выделяем массив указателей: как массив строк
  matrix_ = new double*[rows_];

  // Для каждой строки выделяем массив чисел
  for (int i = 0; i < rows_; ++i) {
    matrix_[i] = new double[cols_]();  // () — обнуляет память
  }
  // Аналог в С:
  // matrix_ = (double**)malloc(rows_ * sizeof(double*));
  // for (int i = 0; i < rows_; i++)
  //     matrix_[i] = (double*)calloc(cols_, sizeof(double));
}

// CopyMatrix() — как deep_copy_matrix() в С
void S21Matrix::CopyMatrix(const S21Matrix& other) {
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] = other.matrix_[i][j];
    }
  }
  // Как в С:
  // for (int i = 0; i < rows; i++)
  //     for (int j = 0; j < cols; j++)
  //         dest->data[i][j] = src->data[i][j];
}

}  // namespace s21
```

---

### ✅ 3.3. Реализация в `destructors.cpp` — как `free()` в С

```cpp
// src/destructors.cpp
#include "s21_matrix_oop.h"

namespace s21 {

// Деструктор — вызывается автоматически при выходе из области видимости
// Как если бы ты вызвал free_matrix() в С — но не нужно вызывать вручную!
S21Matrix::~S21Matrix() {
  FreeMatrix();
}

// FreeMatrix() — как функция освобождения памяти в С
void S21Matrix::FreeMatrix() {
  if (matrix_) {  // Проверка на nullptr — как в С
    for (int i = 0; i < rows_; ++i) {
      delete[] matrix_[i];  // Удаляем каждую строку — как free(row)
    }
    delete[] matrix_;  // Удаляем массив указателей — как free(matrix)
    matrix_ = nullptr;  // Обнуляем — как в С
  }
  // Аналог в С:
  // void free_matrix(Matrix* m) {
  //     for (int i = 0; i < m->rows; i++) free(m->data[i]);
  //     free(m->data);
  //     m->data = NULL;
  // }
}

}  // namespace s21
```

---

### ✅ 3.4. Сравнение: С vs C++

| Задача | В С | В C++ |
|-------|-----|--------|
| Создать матрицу | `Matrix* m = create_matrix(3, 3);` | `S21Matrix m(3, 3);` |
| Освободить память | `free_matrix(m); free(m);` | Автоматически в деструкторе |
| Копировать | `Matrix* copy = copy_matrix(orig);` | `S21Matrix copy = orig;` |
| Переместить | Нет встроенного способа | `S21Matrix m2 = std::move(m1);` |
| Ошибка доступа | Сегфолт | Можно бросить исключение |
| Утечка памяти | Легко, если забыл `free` | Почти невозможно — RAII |

---

### 🔍 Пример: RAII — почему это важно

В С:

```c
Matrix* m = create_matrix(1000, 1000);
if (some_error) {
    return;  // Утечка! Память не освобождена
}
```

В C++:

```cpp
S21Matrix m(1000, 1000);
if (some_error) {
    return;  // Деструктор вызывается автоматически → память чиста
}
```

> ✅ **RAII (Resource Acquisition Is Initialization)** — ключевая идея C++:  
> **Ресурс захватывается в конструкторе, освобождается в деструкторе.**

---

### ✅ 3.5. Что нужно понять новичку

| Концепция | Объяснение |
|----------|-----------|
| `new` / `delete` | Аналог `malloc` / `free`, но вызывает конструктор/деструктор |
| `new[]` / `delete[]` | Для массивов — **всегда парные** |
| Конструктор | Как `init()` — но вызывается автоматически |
| Деструктор | Как `cleanup()` — вызывается, когда объект "умирает" |
| `nullptr` | Современный `NULL` — безопаснее |
| `noexcept` | Гарантирует, что функция не бросит исключение — важно для move |
| `const` в аргументе | `const S21Matrix&` — ссылка, не копия, и не изменяется |

---

### 📚 Что изучить дополнительно:

| Тема | Сравнение с С |
|------|----------------|
| [RAII](https://en.cppreference.com/w/cpp/language/raii) | Как `init` + `cleanup`, но автоматически |
| [new/delete](https://en.cppreference.com/w/cpp/memory/new/operator_new) | `malloc` + `constructor` |
| [Деструктор](https://en.cppreference.com/w/cpp/language/destructor) | `free`, но автоматически |
| [Ссылки (`&`)](https://en.cppreference.com/w/cpp/language/reference) | Как указатели, но безопаснее, не `NULL` |
| [Move semantics](https://stackoverflow.com/questions/3106110/what-are-move-semantics) | Как "переадресация" памяти — эффективно |

---

### ✅ Проверка

Убедись, что файлы созданы:

```bash
ls -l constructors.cpp destructors.cpp
```

---

✅ **Этап 3 завершён.**  
Теперь ты понимаешь:
- Как **управление памятью в C++ проще и безопаснее**, чем в С.
- Что **конструктор и деструктор** — как автоматические `init` и `free`.
- Что **RAII** — главный инструмент против утечек.

---

- [Назад к оглавлению](README.md)
- [📘 Этап 4: Геттеры, сеттеры и арифметические операции — как в С, но удобнее](chapter4.md)