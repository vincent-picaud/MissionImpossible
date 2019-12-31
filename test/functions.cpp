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
