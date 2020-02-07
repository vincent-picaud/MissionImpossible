#include "MissionImpossible/ad.hpp"

#include <gtest/gtest.h>
#include <vector>

using namespace MissionImpossible;

TEST(AD, basic)
{
  AD<double> x1(1), x2(2);

  EXPECT_EQ(x1.index(), 0);
  EXPECT_EQ(x1.value(), 1);

  EXPECT_EQ(x2.index(), 1);
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

TEST(AD, underying_value)
{
  AD<AD<double>> x = 2, y;

  y = 10 * x * x;

  EXPECT_EQ(underlying_value(y), 40);
  EXPECT_EQ(underlying_value(y.value()), 40);
  EXPECT_EQ(underlying_value(y.value().value()), 40);
}
