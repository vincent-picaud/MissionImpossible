#include "AutoDiffCpp/functions.hpp"
#include "AutoDiffCpp/derivatives.hpp"

#include <gtest/gtest.h>
#include <vector>

using namespace AutoDiffCpp;

TEST(Function, exp)
{
  AD<double> x = 2, y;
  y            = exp(x);
  auto grad    = gradient(y);
  ASSERT_DOUBLE_EQ(y.value(), exp(x.value()));
  ASSERT_DOUBLE_EQ(grad[x], exp(x.value()));
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

TEST(Function, exp_sin)
{
  AD<double> x = 2, y;
  y            = exp(sin(x)) * exp(x);
  auto grad    = gradient(y);
  ASSERT_DOUBLE_EQ(y.value(), exp(sin(x.value())) * exp(x.value()));
  ASSERT_DOUBLE_EQ(grad[x], exp(x.value() + sin(x.value())) * (1 + cos(x.value())));
}
