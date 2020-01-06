#include "Mission_Impossible_AutoDiff/functions.hpp"
#include "Mission_Impossible_AutoDiff/derivatives.hpp"

#include <gtest/gtest.h>
#include <cmath>
#include <vector>

using namespace Mission_Impossible_AutoDiff;

//////////////////////////////////////////////////////////////////

TEST(Function, min)
{
  AD<double> x0 = 2, x1 = 3, y;
  y         = min(2 * x0, x0 + x1);
  auto grad = gradient(y);
  // ASSERT_DOUBLE_EQ(y.value(), exp(x.value()));
  // ASSERT_DOUBLE_EQ(grad[x], exp(x.value()));
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
