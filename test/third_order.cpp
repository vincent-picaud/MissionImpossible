#include "Mission_Impossible_AutoDiff/Mission_Impossible.hpp"

#include <gtest/gtest.h>

using namespace Mission_Impossible_AutoDiff;

TEST(Third_Order, Rosenbrock)
{
  AD<AD<AD<double>>> x0(3), x1(4), y;

  y = (1 - x0) * (1 - x0) + 10 * (x1 - x0 * x0) * (x1 - x0 * x0);

  auto grad = gradient(y);

  auto Hessian_x0_row = gradient(grad[x0]);
  auto Hessian_x1_row = gradient(grad[x1]);

  auto third_order_x0_x0_row = gradient(Hessian_x0_row[x0]);
  auto third_order_x0_x1_row = gradient(Hessian_x0_row[x1]);
  auto third_order_x1_x0_row = gradient(Hessian_x1_row[x0]);
  auto third_order_x1_x1_row = gradient(Hessian_x1_row[x1]);

  EXPECT_EQ(y, 254);

  EXPECT_EQ(grad[x0], 604);
  EXPECT_EQ(grad[x1], -100);

  EXPECT_EQ(Hessian_x0_row[x0], +922);
  EXPECT_EQ(Hessian_x0_row[x1], -120);
  EXPECT_EQ(Hessian_x1_row[x0], -120);
  EXPECT_EQ(Hessian_x1_row[x1], 20);

  EXPECT_EQ(third_order_x0_x0_row[x0], 720);
  EXPECT_EQ(third_order_x0_x0_row[x1], -40);
  EXPECT_EQ(third_order_x0_x1_row[x0], -40);
  EXPECT_EQ(third_order_x0_x1_row[x1], 0);
  EXPECT_EQ(third_order_x1_x0_row[x0], -40);
  EXPECT_EQ(third_order_x1_x0_row[x1], 0);
  EXPECT_EQ(third_order_x1_x1_row[x0], 0);
  EXPECT_EQ(third_order_x1_x1_row[x1], 0);
}
