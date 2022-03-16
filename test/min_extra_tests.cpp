#include <MissionImpossible/MissionImpossible.hpp>

#include <gtest/gtest.h>

using namespace MissionImpossible;

TEST(AD, min)
{
  AD<double> x1(1), x2(2);

  EXPECT_EQ(x1, min(x1, x2));
  EXPECT_EQ(x2, max(x1, x2));

  EXPECT_EQ(x1, min(x2, x1));
  EXPECT_EQ(x2, max(x2, x1));
}

TEST(AD, min_nested)
{
  AD<AD<double>> x1(1), x2(2);

  EXPECT_EQ(x1, min(x1, x2));
  EXPECT_EQ(x2, max(x1, x2));

  EXPECT_EQ(x1, min(x2, x1));
  EXPECT_EQ(x2, max(x2, x1));
}

TEST(AD, min_mixt)
{
  double x1 = 1;
  AD<double> x2(2);

  EXPECT_EQ(x1, min(x1, x2));
  EXPECT_EQ(x2, max(x1, x2));

  EXPECT_EQ(x1, min(x2, x1));
  EXPECT_EQ(x2, max(x2, x1));
}

TEST(AD, min_mixt_nested)
{
  double x1 = 1;
  AD<AD<double>> x2(2);

  EXPECT_EQ(x1, min(x1, x2));
  EXPECT_EQ(x2, max(x1, x2));

  EXPECT_EQ(x1, min(x2, x1));
  EXPECT_EQ(x2, max(x2, x1));
}
