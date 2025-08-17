// src/tests/arithmetic_ops_test.cpp
#include <gtest/gtest.h>

#include "s21_matrix_oop.h"

using s21::S21Matrix;

TEST(ArithmeticTest, SumMatrix) {
  S21Matrix m1(2, 2), m2(2, 2);
  m1(0, 0) = 1;
  m2(0, 0) = 2;
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
    for (int j = 0; j < 3; ++j) m1(i, j) = 1;
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 2; ++j) m2(i, j) = 1;

  m1.MulMatrix(m2);
  EXPECT_EQ(m1(0, 0), 3);
}
