// src/s21_matrix_oop.h
#ifndef S21_MATRIX_OOP_H
#define S21_MATRIX_OOP_H

#include <stdexcept>

namespace s21 {

class S21Matrix {
 public:
  // === Конструкторы и деструктор ===
  S21Matrix();
  S21Matrix(int rows, int cols);
  S21Matrix(const S21Matrix& other);
  S21Matrix(S21Matrix&& other) noexcept;
  ~S21Matrix();

  // === Accessors и Mutators ===
  int get_rows() const;
  int get_cols() const;
  void set_rows(int rows);
  void set_cols(int cols);

  // === Основные операции ===
  bool EqMatrix(const S21Matrix& other) const;
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(const double num);
  void MulMatrix(const S21Matrix& other);
  S21Matrix Transpose() const;
  S21Matrix CalcComplements() const;
  double Determinant() const;
  S21Matrix InverseMatrix() const;

  // === Операторы ===
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

 private:
  double** matrix_;
  int rows_;
  int cols_;

  // Вспомогательные методы
  void AllocateMatrix();
  void FreeMatrix();
  void CopyMatrix(const S21Matrix& other);
  double CalcDeterminant() const;
  S21Matrix GetMinorMatrix(int row, int col) const;
};

// Умножение числа на матрицу
S21Matrix operator*(const double num, const S21Matrix& matrix);

}  // namespace s21

#endif  // S21_MATRIX_OOP_H
