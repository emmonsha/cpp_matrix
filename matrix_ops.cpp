// src/matrix_ops.cpp
#include "s21_matrix_oop.h"

namespace s21 {

S21Matrix S21Matrix::Transpose() const {
  S21Matrix result(cols_, rows_);
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      result.matrix_[j][i] = matrix_[i][j];
    }
  }
  return result;
}

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
      result.matrix_[i][j] = det * ((i + j) % 2 == 0 ? 1 : -1);
    }
  }
  return result;
}

double S21Matrix::Determinant() const {
  if (rows_ != cols_) {
    throw std::out_of_range("Matrix must be square to calculate determinant.");
  }
  return CalcDeterminant();
}

S21Matrix S21Matrix::InverseMatrix() const {
  double det = Determinant();
  if (std::abs(det) < 1e-7) {
    throw std::out_of_range("Matrix is singular (determinant is zero).");
  }

  S21Matrix complements = CalcComplements();
  S21Matrix transposed = complements.Transpose();
  transposed.MulNumber(1.0 / det);
  return transposed;
}

double S21Matrix::CalcDeterminant() const {
  if (rows_ == 1) return matrix_[0][0];
  if (rows_ == 2)
    return matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];

  double det = 0;
  for (int j = 0; j < cols_; ++j) {
    S21Matrix minor = GetMinorMatrix(0, j);
    double cofactor = minor.Determinant() * (j % 2 == 0 ? 1 : -1);
    det += matrix_[0][j] * cofactor;
  }
  return det;
}

S21Matrix S21Matrix::GetMinorMatrix(int row, int col) const {
  S21Matrix minor(rows_ - 1, cols_ - 1);
  int minor_i = 0;
  for (int i = 0; i < rows_; ++i) {
    if (i == row) continue;
    int minor_j = 0;
    for (int j = 0; j < cols_; ++j) {
      if (j == col) continue;
      minor.matrix_[minor_i][minor_j] = matrix_[i][j];
      ++minor_j;
    }
    ++minor_i;
  }
  return minor;
}

}  // namespace s21
