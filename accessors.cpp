// src/accessors.cpp
#include <algorithm>

#include "s21_matrix_oop.h"

namespace s21 {

int S21Matrix::get_rows() const { return rows_; }

int S21Matrix::get_cols() const { return cols_; }

void S21Matrix::set_rows(int rows) {
  if (rows <= 0) {
    throw std::out_of_range("Rows must be positive.");
  }
  if (rows == rows_) return;

  double** new_matrix = new double*[rows];
  for (int i = 0; i < rows; ++i) {
    new_matrix[i] = new double[cols_]();
  }

  int copy_rows = std::min(rows, rows_);
  for (int i = 0; i < copy_rows; ++i) {
    std::copy(matrix_[i], matrix_[i] + cols_, new_matrix[i]);
  }

  FreeMatrix();
  matrix_ = new_matrix;
  rows_ = rows;
}

void S21Matrix::set_cols(int cols) {
  if (cols <= 0) {
    throw std::out_of_range("Cols must be positive.");
  }
  if (cols == cols_) return;

  double** new_matrix = new double*[rows_];
  for (int i = 0; i < rows_; ++i) {
    new_matrix[i] = new double[cols]();
    std::copy(matrix_[i], matrix_[i] + std::min(cols, cols_), new_matrix[i]);
  }

  FreeMatrix();
  matrix_ = new_matrix;
  cols_ = cols;
}

}  // namespace s21
