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
