// src/tests/operators_test.cpp
#include <gtest/gtest.h>

#include "s21_matrix_oop.h"

using s21::S21Matrix;

TEST(OperatorsTest, PlusOperator) {
  S21Matrix m1(2, 2), m2(2, 2);
  m1(0, 0) = 1;
  m2(0, 0) = 1;
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
