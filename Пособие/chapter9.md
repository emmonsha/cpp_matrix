
---

## 📘 Этап 9: Генерация документации с Doxygen

> 💡 Doxygen — это как **Javadoc для C++**.  
> Он читает специальные комментарии в коде и создаёт:
> - HTML-сайт с описанием классов
> - Диаграммы наследования
> - Список методов, полей, параметров
> - Поиск по коду

---

## ✅ Шаг 1: Установка Doxygen

```bash
sudo apt install doxygen graphviz -y
```

> - `doxygen` — основной инструмент
> - `graphviz` — для генерации диаграмм (опционально)

Проверка:
```bash
doxygen --version
```

---

## ✅ Шаг 2: Создай конфигурационный файл `Doxyfile`

В папке `src/` выполните:

```bash
doxygen -g Doxyfile
```

> Создастся файл `Doxyfile` с настройками.

---

## ✅ Шаг 3: Настройка `Doxyfile`

Открой `Doxyfile` и найди и измени следующие строки:

```txt
PROJECT_NAME           = "S21Matrix"
PROJECT_BRIEF          = "C++ Matrix Library with OOP and Unit Tests"
OUTPUT_DIRECTORY       = docs
CREATE_SUBDIRS         = YES
EXTRACT_ALL            = YES
EXTRACT_PRIVATE        = YES
EXTRACT_STATIC         = YES
RECURSIVE              = YES
EXCLUDE                = build lib tests
SOURCE_BROWSER         = YES
INLINE_SOURCES         = NO
ALPHABETICAL_INDEX     = YES
GENERATE_HTML          = YES
HTML_OUTPUT            = html
GENERATE_LATEX         = NO
GENERATE_MAN           = NO
GENERATE_RTF           = NO
CLASS_DIAGRAMS         = YES
CLASS_GRAPH            = YES
COLLABORATION_GRAPH    = YES
INCLUDE_GRAPH          = YES
INCLUDED_BY_GRAPH      = YES
CALL_GRAPH             = NO
DIRECTORY_GRAPH        = YES
DOT_GRAPH_MAX_NODES    = 500
HAVE_DOT               = YES
CLASS_VISIBILITY_LINES = YES
TEMPLATE_RELATIONS     = YES
INCLUDE_FILE_PATTERNS  = *.h *.cpp
ENABLE_PREPROCESSING   = YES
SEARCH_INCLUDES        = YES
```

> ✅ Это минимальная полезная конфигурация:
> - Документация в папке `docs/html`
> - Показываются все методы и поля
> - Генерируются диаграммы
> - Исключены `build`, `lib`, `tests`

---

## ✅ Шаг 4: Добавь комментарии в `s21_matrix_oop.h`

Doxygen читает комментарии вида `///` или `/** */`.

Обнови `s21_matrix_oop.h`:

```cpp
// src/s21_matrix_oop.h
/**
 * @file s21_matrix_oop.h
 * @brief Declaration of the S21Matrix class for matrix operations.
 * @author Your Name
 * @date 2025
 */

#ifndef S21_MATRIX_OOP_H
#define S21_MATRIX_OOP_H

#include <stdexcept>

/// @namespace s21
/// @brief Namespace for the S21 matrix library.
namespace s21 {

/**
 * @class S21Matrix
 * @brief A class for matrix operations using OOP principles.
 *
 * This class supports matrix creation, arithmetic operations,
 * transposition, determinant calculation, and inversion.
 * It uses RAII for memory management.
 */
class S21Matrix {
 public:
  /// @name Constructors and Destructor
  /// @{

  /**
   * @brief Default constructor creates a 3x3 zero matrix.
   */
  S21Matrix();

  /**
   * @brief Parameterized constructor.
   * @param rows Number of rows (must be > 0)
   * @param cols Number of columns (must be > 0)
   * @throw std::out_of_range if rows or cols <= 0
   */
  S21Matrix(int rows, int cols);

  /**
   * @brief Copy constructor.
   * @param other Matrix to copy
   */
  S21Matrix(const S21Matrix& other);

  /**
   * @brief Move constructor.
   * @param other Matrix to move (rvalue reference)
   */
  S21Matrix(S21Matrix&& other) noexcept;

  /**
   * @brief Destructor.
   * Frees allocated memory.
   */
  ~S21Matrix();

  /// @}

  /// @name Accessors and Mutators
  /// @{

  /**
   * @brief Get number of rows.
   * @return Number of rows
   */
  int get_rows() const;

  /**
   * @brief Get number of columns.
   * @return Number of columns
   */
  int get_cols() const;

  /**
   * @brief Set number of rows.
   * @param rows New number of rows (> 0)
   * @throw std::out_of_range if rows <= 0
   */
  void set_rows(int rows);

  /**
   * @brief Set number of columns.
   * @param cols New number of columns (> 0)
   * @throw std::out_of_range if cols <= 0
   */
  void set_cols(int cols);

  /// @}

  /// @name Matrix Operations
  /// @{

  /**
   * @brief Check if two matrices are equal.
   * @param other Matrix to compare
   * @return true if matrices are equal (element-wise, with epsilon)
   */
  bool EqMatrix(const S21Matrix& other) const;

  /**
   * @brief Add another matrix to this one.
   * @param other Matrix to add
   * @throw std::out_of_range if dimensions don't match
   */
  void SumMatrix(const S21Matrix& other);

  /**
   * @brief Subtract another matrix from this one.
   * @param other Matrix to subtract
   * @throw std::out_of_range if dimensions don't match
   */
  void SubMatrix(const S21Matrix& other);

  /**
   * @brief Multiply matrix by a scalar.
   * @param num Scalar value
   */
  void MulNumber(const double num);

  /**
   * @brief Multiply this matrix by another.
   * @param other Right-hand matrix
   * @throw std::out_of_range if cols != other.rows
   */
  void MulMatrix(const S21Matrix& other);

  /**
   * @brief Transpose the matrix.
   * @return New transposed matrix
   */
  S21Matrix Transpose() const;

  /**
   * @brief Calculate matrix of cofactors.
   * @return Matrix of algebraic complements
   * @throw std::out_of_range if matrix is not square
   */
  S21Matrix CalcComplements() const;

  /**
   * @brief Calculate determinant.
   * @return Determinant value
   * @throw std::out_of_range if matrix is not square
   */
  double Determinant() const;

  /**
   * @brief Calculate inverse matrix.
   * @return Inverse matrix
   * @throw std::out_of_range if determinant is zero
   */
  S21Matrix InverseMatrix() const;

  /// @}

  /// @name Operator Overloading
  /// @{

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
  double& operator()(int i, int j);
  const double& operator()(int i, int j) const;

  /// @}

 private:
  double** matrix_;  ///< 2D array of matrix elements
  int rows_;         ///< Number of rows
  int cols_;         ///< Number of columns

  // Helper methods
  void AllocateMatrix();
  void FreeMatrix();
  void CopyMatrix(const S21Matrix& other);
  double CalcDeterminant() const;
  S21Matrix GetMinorMatrix(int row, int col) const;
};

/**
 * @brief Multiply scalar by matrix.
 * @param num Scalar value
 * @param matrix Matrix
 * @return Result of multiplication
 */
S21Matrix operator*(const double num, const S21Matrix& matrix);

}  // namespace s21

#endif  // S21_MATRIX_OOP_H
```

---

## ✅ Шаг 5: Генерация документации

Выполни в папке `src/`:

```bash
doxygen Doxyfile
```

---

## ✅ Шаг 6: Просмотр документации

Открой в браузере:

```bash
firefox docs/html/index.html
```

или

```bash
xdg-open docs/html/index.html
```

---

## ✅ Что ты увидишь

- Главная страница с описанием проекта
- Класс `S21Matrix` с полным списком методов
- Диаграмму класса (если установлен `graphviz`)
- Описание каждого метода, параметров, исключений
- Поиск по коду

---

## ✅ Добавим в `Makefile` цель `docs`

Добавь в `Makefile`:

```makefile
.PHONY: docs

docs:
	@echo "📚 Generating documentation with Doxygen..."
	doxygen Doxyfile
	@echo "Documentation generated in docs/html/"
```

Теперь можно:
```bash
make docs
```

---

## 📚 Что изучить дополнительно:

| Тема | Ресурс |
|------|--------|
| [Doxygen Manual](https://www.doxygen.nl/manual/) | Полная документация |
| [Doxygen Commands](https://www.doxygen.nl/manual/commands.html) | `@brief`, `@param`, `@return`, `@throw` |
| [Markdown in Doxygen](https://www.doxygen.nl/manual/markdown.html) | Поддержка Markdown |
| [Custom CSS](https://www.doxygen.nl/manual/customize.html) | Стилизация документации |

---

- [Назад к оглавлению](README.md)
- [📎 1. `Doxyfile`  📄 2. `README.md`](10.Doxyfile.md)