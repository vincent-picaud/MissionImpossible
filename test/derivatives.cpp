#include "AutoDiffCpp/derivatives.hpp"

#include <gtest/gtest.h>

using namespace AutoDiffCpp;

TEST(Derivatives, product)
{
  AD<double> x(3), y;

  y = x * x;

  auto grad = gradient(y);

  EXPECT_EQ(grad[x], 6);
  EXPECT_EQ(grad[y], 1);
}
