// src/tests/constructors_test.cpp
#include <gtest/gtest.h>

#include "s21_matrix_oop.h"

using s21::S21Matrix;

TEST(ConstructorTest, DefaultConstructor) {
  S21Matrix m;
  EXPECT_EQ(m.get_rows(), 3);
  EXPECT_EQ(m.get_cols(), 3);
}

TEST(ConstructorTest, ParamConstructor) {
  S21Matrix m(2, 3);
  EXPECT_EQ(m.get_rows(), 2);
  EXPECT_EQ(m.get_cols(), 3);
}

TEST(ConstructorTest, CopyConstructor) {
  S21Matrix m1(2, 2);
  m1(0, 0) = 1;
  S21Matrix m2(m1);
  EXPECT_EQ(m2(0, 0), 1);
}

TEST(ConstructorTest, MoveConstructor) {
  S21Matrix m1(2, 2);
  m1(0, 0) = 5;
  S21Matrix m2(std::move(m1));
  EXPECT_EQ(m2(0, 0), 5);
  EXPECT_EQ(m1.get_rows(), 0);
}
