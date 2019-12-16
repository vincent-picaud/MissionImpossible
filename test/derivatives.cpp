#include "AutoDiffCpp/derivatives.hpp"

#include <gtest/gtest.h>

using namespace AutoDiffCpp;

TEST(Derivatives, product)
{
  AD<double> x0(3), x1(4), y;

  y = 2 * x0;

  auto grad = gradient(y);
  EXPECT_EQ(y.value(), 6);
  EXPECT_EQ(grad[x0], 2);
  EXPECT_EQ(grad[x1], 0);
  EXPECT_EQ(grad[y], 1);

  y = 2 * x0 * x1;

  grad = gradient(y);
  EXPECT_EQ(y.value(), 24);
  EXPECT_EQ(grad[x0], 8);
  EXPECT_EQ(grad[x1], 6);
  EXPECT_EQ(grad[y], 1);
}

TEST(Derivatives, substraction)
{
  AD<double> x0(3), x1(4), y;

  y = 2 - x0;

  auto grad = gradient(y);
  EXPECT_EQ(y.value(), -1);
  EXPECT_EQ(grad[x0], -1);
  EXPECT_EQ(grad[x1], 0);
  EXPECT_EQ(grad[y], 1);

  y = 2 * x0 - x1;

  grad = gradient(y);
  EXPECT_EQ(y.value(), 2);
  EXPECT_EQ(grad[x0], 2);
  EXPECT_EQ(grad[x1], -1);
  EXPECT_EQ(grad[y], 1);

  y = 2 * x0 - 4 * x1;

  grad = gradient(y);
  EXPECT_EQ(y.value(), -10);
  EXPECT_EQ(grad[x0], 2);
  EXPECT_EQ(grad[x1], -4);
  EXPECT_EQ(grad[y], 1);
}

TEST(Derivatives, addition)
{
  AD<double> x0(3), x1(4), y;

  y = 2 + x0;

  auto grad = gradient(y);
  EXPECT_EQ(y.value(), 5);
  EXPECT_EQ(grad[x0], +1);
  EXPECT_EQ(grad[x1], 0);
  EXPECT_EQ(grad[y], 1);

  y = 2 * x0 + x1;

  grad = gradient(y);
  EXPECT_EQ(y.value(), 10);
  EXPECT_EQ(grad[x0], 2);
  EXPECT_EQ(grad[x1], +1);
  EXPECT_EQ(grad[y], 1);

  y = 2 * x0 + 4 * x1;

  grad = gradient(y);
  EXPECT_EQ(y.value(), 22);
  EXPECT_EQ(grad[x0], 2);
  EXPECT_EQ(grad[x1], 4);
  EXPECT_EQ(grad[y], 1);
}

TEST(Derivatives, random_1)
{
  AD<double> x0(3), x1(4), y;

  y = (x0 - x1) * (x0 + x1);  // x0^2-x1^2

  auto grad = gradient(y);
  EXPECT_EQ(y.value(), -7);
  EXPECT_EQ(grad[x0], 6);
  EXPECT_EQ(grad[x1], -8);
  EXPECT_EQ(grad[y], 1);

  y = x0 * x0 - x1 * x1;

  grad = gradient(y);
  EXPECT_EQ(y.value(), -7);
  EXPECT_EQ(grad[x0], 6);
  EXPECT_EQ(grad[x1], -8);
  EXPECT_EQ(grad[y], 1);
}

TEST(Derivatives, random_2)
{
  AD<double> x0(3), x1(4), y;

  y = (1 - x0) * (1 - x0) + 10 * (x1 - x0 * x0) * (x1 - x0 * x0);

  auto grad = gradient(y);
  EXPECT_EQ(y.value(), 254);
  EXPECT_EQ(grad[x0], 604);
  EXPECT_EQ(grad[x1], -100);
  EXPECT_EQ(grad[y], 1);

  auto column_x0 = Jacobian_column(x0);
  EXPECT_EQ(column_x0[x0], 1);
  EXPECT_EQ(column_x0[x1], 0);
  EXPECT_EQ(column_x0[y], 604);

  auto column_x1 = Jacobian_column(x1);
  EXPECT_EQ(column_x1[x0], 0);
  EXPECT_EQ(column_x1[x1], 1);
  EXPECT_EQ(column_x1[y], -100);

  auto column_y = Jacobian_column(y);
  EXPECT_EQ(column_y[x0], 0);
  EXPECT_EQ(column_y[x1], 0);
  EXPECT_EQ(column_y[y], 1);
}
