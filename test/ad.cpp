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

TEST(AD, scalar_product)
{
  AD<double> x(2);

  auto z = 3 * x;

  EXPECT_EQ(z.value(), 6);
}

TEST(AD, ad_product)
{
  AD<double> x(2);

  auto z = x * x;

  EXPECT_EQ(z.value(), 4);
}
