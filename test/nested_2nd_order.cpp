#include "AutoDiffCpp/ad.hpp"
#include "AutoDiffCpp/derivatives.hpp"

#include <gtest/gtest.h>
#include <vector>

using namespace AutoDiffCpp;

TEST(Nested, Rosenbrock)
{
  AD<AD<double>> x0(3), x1(4), y;

  y = (1 - x0) * (1 - x0) +
      10 * (x1 - x0 * x0) * (x1 - x0 * x0);  // remark: y[x0] type is  AD<AD<T>>

  EXPECT_EQ(y.value().value(), 254);  // TOFIX: overload == to avoid value().value()

  auto y_gradient = Jacobian_row(y);

  EXPECT_EQ(y_gradient[x0].value(), 604);  // remark: y_gradient[x0] type is AD<T>
  EXPECT_EQ(y_gradient[x1].value(), -100);

  auto Hessian_x0_row = Jacobian_row(y_gradient[x0]);  // remark: Hessian_x0_row[x0] type is T

  EXPECT_EQ(Hessian_x0_row[x0.value()], 922);  // CAVEAT: Concerning
                                               // the ugly syntax
                                               // [x0.value()] for the
                                               // moment I do not
                                               // think we must
                                               // redefine a
                                               // Tape_Vector::op[]()
                                               // BUT one must define
                                               // a new Tape_Matrix
                                               // type instead
  EXPECT_EQ(Hessian_x0_row[x1.value()], -120);

  auto Hessian_x1_row = Jacobian_row(y_gradient[x1]);

  EXPECT_EQ(Hessian_x1_row[x0.value()], -120);
  EXPECT_EQ(Hessian_x1_row[x1.value()], 20);
}
