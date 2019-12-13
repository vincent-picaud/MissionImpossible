#include "AutoDiffCpp/ad.hpp"

#include <gtest/gtest.h>
#include <vector>

using namespace AutoDiffCpp;

TEST(AD, basic)
{
  AD<double> x1(1), x2(2);

  EXPECT_EQ(x1.index()[0], 0);
  EXPECT_EQ(x1.value(), 1);

  EXPECT_EQ(x2.index()[0], 1);
  EXPECT_EQ(x2.value(), 2);
}

TEST(AD, product)
{
  AD<double> x(2);

  auto z0 = 3 * x;

  EXPECT_EQ(z0.value(), 6);

  auto z1 = x * x;

  EXPECT_EQ(z1.value(), 4);

  auto z2 = (x * x) * (x * x);

  EXPECT_EQ(z2.value(), 16);
}
