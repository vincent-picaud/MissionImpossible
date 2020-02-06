#include "MissionImpossible/functions.hpp"
#include "MissionImpossible/derivatives.hpp"

#include <gtest/gtest.h>
#include <cmath>
#include <vector>

using namespace MissionImpossible;

//////////////////////////////////////////////////////////////////

TEST(Function, min)
{
  AD<double> x0 = 2, x1 = 3, y;
  y         = min(2 * x0, x0 + x1);
  auto grad = gradient(y);
  ASSERT_DOUBLE_EQ(y.value(), 4);
  ASSERT_DOUBLE_EQ(grad[x0], 2);
  ASSERT_DOUBLE_EQ(grad[x1], 0);

  y    = min(4, x0 + x1);
  grad = gradient(y);
  ASSERT_DOUBLE_EQ(y.value(), 4);
  ASSERT_DOUBLE_EQ(grad[x0], 0);
  ASSERT_DOUBLE_EQ(grad[x1], 0);

  y    = min(10, 2 * x0 + x1);
  grad = gradient(y);
  ASSERT_DOUBLE_EQ(y.value(), 7);
  ASSERT_DOUBLE_EQ(grad[x0], 2);
  ASSERT_DOUBLE_EQ(grad[x1], 1);

  // y    = max(2 * x0, x0 + x1);
  // grad = gradient(y);
  // ASSERT_DOUBLE_EQ(y.value(), 4);
  // ASSERT_DOUBLE_EQ(grad[x0], 2);
  // ASSERT_DOUBLE_EQ(grad[x1], 0);
}

TEST(Function, max)
{
  AD<double> x0 = 2, x1 = 3, y;
  y         = max(2 * x0, x0 + 2*x1);
  auto grad = gradient(y);
  ASSERT_DOUBLE_EQ(y.value(), 8);
  ASSERT_DOUBLE_EQ(grad[x0], 1);
  ASSERT_DOUBLE_EQ(grad[x1], 2);

  y    = max(6, x0 + x1);
  grad = gradient(y);
  ASSERT_DOUBLE_EQ(y.value(), 6);
  ASSERT_DOUBLE_EQ(grad[x0], 0);
  ASSERT_DOUBLE_EQ(grad[x1], 0);

  y    = max(1, 2 * x0 + x1);
  grad = gradient(y);
  ASSERT_DOUBLE_EQ(y.value(), 7);
  ASSERT_DOUBLE_EQ(grad[x0], 2);
  ASSERT_DOUBLE_EQ(grad[x1], 1);
}

//////////////////////////////////////////////////////////////////

TEST(Function, pow)
{
  AD<double> x = 2, y;
  y            = pow(x, 3);
  auto grad    = gradient(y);

  ASSERT_DOUBLE_EQ(y.value(), pow(x.value(), 3));
  ASSERT_DOUBLE_EQ(grad[x], 3 * pow(x.value(), 2));
}

TEST(Function, exp)
{
  AD<double> x = 2, y;
  y            = exp(x);
  auto grad    = gradient(y);
  ASSERT_DOUBLE_EQ(y.value(), exp(x.value()));
  ASSERT_DOUBLE_EQ(grad[x], exp(x.value()));
}

TEST(Function, log_exp)
{
  AD<double> x = 2, y;
  y            = log(exp(x));

  auto grad = gradient(y);
  ASSERT_DOUBLE_EQ(y.value(), x.value());
  ASSERT_DOUBLE_EQ(grad[x], 1);
}

TEST(Function, sin)
{
  AD<double> x = 2, y;
  y            = sin(x);
  auto grad    = gradient(y);
  ASSERT_DOUBLE_EQ(y.value(), sin(x.value()));
  ASSERT_DOUBLE_EQ(grad[x], cos(x.value()));
}

TEST(Function, cos)
{
  AD<double> x = 2, y;
  y            = cos(x);
  auto grad    = gradient(y);
  ASSERT_DOUBLE_EQ(y.value(), cos(x.value()));
  ASSERT_DOUBLE_EQ(grad[x], -sin(x.value()));
}

TEST(Function, tan)
{
  AD<double> x = 2, y;
  y            = tan(x);

  auto grad = gradient(y);
  ASSERT_DOUBLE_EQ(y.value(), tan(x.value()));
  ASSERT_DOUBLE_EQ(grad[x], 1 / pow(cos(x.value()), 2));
}
TEST(Function, exp_sin)
{
  AD<double> x = 2, y;
  y            = exp(sin(x)) * exp(x);
  auto grad    = gradient(y);
  ASSERT_DOUBLE_EQ(y.value(), exp(sin(x.value())) * exp(x.value()));
  ASSERT_DOUBLE_EQ(grad[x], exp(x.value() + sin(x.value())) * (1 + cos(x.value())));
}

//////////////////////////////////////////////////////////////////

TEST(Function, sigmoid)
{
  AD<double> x = 2, y;
  y            = 1 / (1 + exp(-x));

  auto grad = gradient(y);

  ASSERT_DOUBLE_EQ(y.value(), 1 / (1 + exp(-x.value())));
  ASSERT_DOUBLE_EQ(grad[x], exp(-x.value()) / pow(1 + exp(-x.value()), 2));
}

//////////////////////////////////////////////////////////////////

TEST(Function, sqrt)
{
  AD<double> x = 3, y;

  y         = sqrt(x * x);
  auto grad = gradient(y);

  ASSERT_DOUBLE_EQ(y.value(), 3);
  ASSERT_DOUBLE_EQ(grad[x], 1);

  x    = -3;
  y    = sqrt(x * x);
  grad = gradient(y);

  ASSERT_DOUBLE_EQ(y.value(), 3);
  ASSERT_DOUBLE_EQ(grad[x], -1);
}
