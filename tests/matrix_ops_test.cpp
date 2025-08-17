// src/tests/matrix_ops_test.cpp
#include <gtest/gtest.h>

#include "s21_matrix_oop.h"

using s21::S21Matrix;

TEST(MatrixOpsTest, Transpose) {
  S21Matrix m(2, 3);
  m(0, 0) = 1;
  m(0, 1) = 2;
  m(0, 2) = 3;
  m(1, 0) = 4;
  m(1, 1) = 5;
  m(1, 2) = 6;

  S21Matrix t = m.Transpose();
  EXPECT_EQ(t(0, 0), 1);
  EXPECT_EQ(t(2, 1), 6);
}

TEST(MatrixOpsTest, Determinant) {
  S21Matrix m(2, 2);
  m(0, 0) = 1;
  m(0, 1) = 2;
  m(1, 0) = 3;
  m(1, 1) = 4;
  EXPECT_NEAR(m.Determinant(), -2, 1e-7);
}

TEST(MatrixOpsTest, InverseMatrix) {
  S21Matrix m(2, 2);
  m(0, 0) = 4;
  m(0, 1) = 7;
  m(1, 0) = 2;
  m(1, 1) = 6;

  S21Matrix inv = m.InverseMatrix();
  S21Matrix identity = m * inv;
  EXPECT_NEAR(identity(0, 0), 1, 1e-7);
  EXPECT_NEAR(identity(1, 1), 1, 1e-7);
}
