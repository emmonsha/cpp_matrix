Отлично! Переходим к **Этапу 7** — написанию **unit-тестов с GTest**, чтобы убедиться, что наша матрица работает правильно.

---

## 📘 Этап 7: Unit-тесты с Google Test — как проверить, что код работает

> 💡 В С ты, возможно, проверял вручную:
> ```c
> printf("Test 1: %s\n", result == expected ? "OK" : "FAIL");
> ```
>
> В C++ есть **Google Test (GTest)** — мощная библиотека для автоматических тестов.  
> Она позволяет писать:
> ```cpp
> TEST(MatrixTest, CanAddMatrices) {
>   S21Matrix a(2, 2), b(2, 2);
>   S21Matrix result = a + b;
>   EXPECT_EQ(result(0, 0), 0);
> }
> ```

---

### 🔧 7.1. Создаём папку для тестов

```bash
mkdir tests
```

---

### 🔧 7.2. Создаём `main_test.cpp` — точка входа для тестов

```bash
touch tests/main_test.cpp
```

```cpp
// src/tests/main_test.cpp
#include <gtest/gtest.h>

// Точка входа для всех тестов
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
```

> ✅ Это стандартный шаблон: инициализация GTest и запуск всех тестов.

---

### 🔧 7.3. Тест конструкторов

```bash
touch tests/constructors_test.cpp
```

```cpp
// src/tests/constructors_test.cpp
#include "s21_matrix_oop.h"
#include <gtest/gtest.h>

using s21::S21Matrix;

// Тест: базовый конструктор
TEST(ConstructorTest, DefaultConstructor) {
  S21Matrix m;
  EXPECT_EQ(m.get_rows(), 3);
  EXPECT_EQ(m.get_cols(), 3);
}

// Тест: параметризированный конструктор
TEST(ConstructorTest, ParamConstructor) {
  S21Matrix m(2, 3);
  EXPECT_EQ(m.get_rows(), 2);
  EXPECT_EQ(m.get_cols(), 3);
}

// Тест: конструктор копирования
TEST(ConstructorTest, CopyConstructor) {
  S21Matrix m1(2, 2);
  m1(0, 0) = 42;
  S21Matrix m2(m1);
  EXPECT_EQ(m2(0, 0), 42);
}

// Тест: move-конструктор
TEST(ConstructorTest, MoveConstructor) {
  S21Matrix m1(2, 2);
  m1(0, 0) = 99;
  S21Matrix m2(std::move(m1));
  EXPECT_EQ(m2(0, 0), 99);
  // m1 больше не должен владеть памятью
}
```

---

### 🔧 7.4. Тест accessors и mutators

```bash
touch tests/accessors_test.cpp
```

```cpp
// src/tests/accessors_test.cpp
#include "s21_matrix_oop.h"
#include <gtest/gtest.h>

using s21::S21Matrix;

TEST(AccessorTest, GetSetRows) {
  S21Matrix m(2, 2);
  m.set_rows(5);
  EXPECT_EQ(m.get_rows(), 5);
}

TEST(AccessorTest, SetRowsPreservesData) {
  S21Matrix m(2, 2);
  m(0, 0) = 1;
  m.set_rows(3);
  EXPECT_EQ(m(0, 0), 1);  // данные остались
  EXPECT_EQ(m(2, 0), 0);  // новая строка — нули
}
```

---

### 🔧 7.5. Тест арифметики

```bash
touch tests/arithmetic_ops_test.cpp
```

```cpp
// src/tests/arithmetic_ops_test.cpp
#include "s21_matrix_oop.h"
#include <gtest/gtest.h>

using s21::S21Matrix;

TEST(ArithmeticTest, SumMatrix) {
  S21Matrix m1(2, 2), m2(2, 2);
  m1(0, 0) = 1; m2(0, 0) = 2;
  m1.SumMatrix(m2);
  EXPECT_EQ(m1(0, 0), 3);
}

TEST(ArithmeticTest, MulNumber) {
  S21Matrix m(2, 2);
  m(0, 0) = 2;
  m.MulNumber(3);
  EXPECT_EQ(m(0, 0), 6);
}

TEST(ArithmeticTest, MulMatrix) {
  S21Matrix m1(2, 3), m2(3, 2);
  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 3; ++j)
      m1(i, j) = 1;
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 2; ++j)
      m2(i, j) = 1;

  m1.MulMatrix(m2);
  EXPECT_EQ(m1(0, 0), 3);  // 1*1 + 1*1 + 1*1 = 3
}
```

---

### 🔧 7.6. Тест матричных операций

```bash
touch tests/matrix_ops_test.cpp
```

```cpp
// src/tests/matrix_ops_test.cpp
#include "s21_matrix_oop.h"
#include <gtest/gtest.h>

using s21::S21Matrix;

TEST(MatrixOpsTest, Transpose) {
  S21Matrix m(2, 3);
  m(0, 0) = 1; m(0, 1) = 2; m(0, 2) = 3;
  m(1, 0) = 4; m(1, 1) = 5; m(1, 2) = 6;

  S21Matrix t = m.Transpose();
  EXPECT_EQ(t(0, 0), 1);
  EXPECT_EQ(t(2, 1), 6);
}

TEST(MatrixOpsTest, Determinant) {
  S21Matrix m(2, 2);
  m(0, 0) = 1; m(0, 1) = 2;
  m(1, 0) = 3; m(1, 1) = 4;
  EXPECT_NEAR(m.Determinant(), -2, 1e-7);
}

TEST(MatrixOpsTest, InverseMatrix) {
  S21Matrix m(2, 2);
  m(0, 0) = 4; m(0, 1) = 7;
  m(1, 0) = 2; m(1, 1) = 6;

  S21Matrix inv = m.InverseMatrix();
  S21Matrix identity = m * inv;

  EXPECT_NEAR(identity(0, 0), 1, 1e-7);
  EXPECT_NEAR(identity(1, 1), 1, 1e-7);
}
```

---

### 🔧 7.7. Тест операторов

```bash
touch tests/operators_test.cpp
```

```cpp
// src/tests/operators_test.cpp
#include "s21_matrix_oop.h"
#include <gtest/gtest.h>

using s21::S21Matrix;

TEST(OperatorsTest, PlusOperator) {
  S21Matrix m1(2, 2), m2(2, 2);
  m1(0, 0) = 1; m2(0, 0) = 1;
  S21Matrix m3 = m1 + m2;
  EXPECT_EQ(m3(0, 0), 2);
}

TEST(OperatorsTest, Indexing) {
  S21Matrix m(2, 2);
  m(1, 1) = 42;
  EXPECT_EQ(m(1, 1), 42);
}

TEST(OperatorsTest, Equality) {
  S21Matrix m1(2, 2), m2(2, 2);
  EXPECT_TRUE(m1 == m2);
  m1(0, 0) = 1;
  EXPECT_FALSE(m1 == m2);
}

TEST(OperatorsTest, ScalarMultiplication) {
  S21Matrix m(2, 2);
  m(0, 0) = 2;
  S21Matrix result = 3 * m;
  EXPECT_EQ(result(0, 0), 6);
}
```

---

### ✅ 7.8. Объяснение для новичка

| Конструкция | Что делает |
|-----------|-----------|
| `TEST(SuiteName, TestName)` | Определяет один тест |
| `EXPECT_EQ(a, b)` | Проверяет равенство |
| `EXPECT_NEAR(a, b, eps)` | Для `double` — с погрешностью |
| `ASSERT_THROW(func(), std::out_of_range)` | Проверяет, что функция бросает исключение |

> ✅ Тесты — это **автоматические проверки**.  
> Если ты изменишь код и сломаешь что-то — тесты **сразу предупредят**.

---

### 📚 Что изучить дополнительно:

| Тема | Ресурс |
|------|--------|
| [Google Test Primer](https://github.com/google/googletest/blob/main/docs/primer.md) | Официальное руководство |
| [ASSERT vs EXPECT](https://stackoverflow.com/questions/6328693/difference-between-assert-and-assert) | `ASSERT` — фатально, `EXPECT` — продолжает |
| [Тестирование исключений](https://github.com/google/googletest/blob/main/docs/reference/assertions.md#exception-assertions) | `EXPECT_THROW`, `EXPECT_ANY_THROW` |

---

### ✅ Проверка

Убедись, что все тесты созданы:

```bash
ls -l tests/
```

---

✅ **Этап 7 завершён.**  
Теперь ты умеешь:
- Писать **автоматические тесты**.
- Проверять **конструкторы, операции, исключения**.
- Использовать **GTest** как профессионал.

---

Готов к **Этапу 8: Сборка проекта с помощью Makefile и запуск всех проверок**?

Напиши: **"Следующий этап"** — и я продолжу.