// src/tests/accessors_test.cpp
#include <gtest/gtest.h>

#include "s21_matrix_oop.h"

using s21::S21Matrix;

TEST(AccessorTest, GetSetRows) {
  S21Matrix m(2, 2);
  m.set_rows(4);
  EXPECT_EQ(m.get_rows(), 4);
  EXPECT_EQ(m.get_cols(), 2);
}

TEST(AccessorTest, GetSetCols) {
  S21Matrix m(2, 2);
  m.set_cols(5);
  EXPECT_EQ(m.get_cols(), 5);
}

TEST(AccessorTest, SetRowsPreservesData) {
  S21Matrix m(2, 2);
  m(0, 0) = 1;
  m.set_rows(3);
  EXPECT_EQ(m(0, 0), 1);
  EXPECT_EQ(m(2, 0), 0);  // zero-padded
}
