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

  EXPECT_EQ(y, 254);  // TOFIX: overload == to avoid value().value() <- CREAT AN EXTRA INSTANCE
                      // So far, fixed with Final_Value_Type
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

  // Check tape length
  //
  EXPECT_EQ(x0.tape().row_size(), 3);
  EXPECT_EQ(x0.value().tape().row_size(), 51);

  EXPECT_EQ(x0.tape().memory_size(), 224);
  EXPECT_EQ(x0.value().tape().memory_size(), 1440);
}

TEST(Nested, Debug_op_eq_must_not_create_a_new_var_nested)
{
  AD<AD<double>> x0(3);
  const std::size_t n_1  = x0.tape().row_size();
  const std::size_t nn_1 = x0.value().tape().row_size();
  const bool ok          = x0 == 3;  // this must not create an instance! NOT OK
  ASSERT_TRUE(ok);
  EXPECT_EQ(n_1, x0.tape().row_size());
  EXPECT_EQ(nn_1, x0.value().tape().row_size());
}

TEST(Nested, Debug_op_eq_must_not_create_a_new_var)
{
  AD<double> x0(3);
  const std::size_t n_1 = x0.tape().row_size();
  const bool ok         = x0 == 3;  // this must not create an instance! OK
  ASSERT_TRUE(ok);
  EXPECT_EQ(n_1, x0.tape().row_size());
}

TEST(Nested, Simple_Polynomial)
{
  AD<AD<double>> x(4), y;
  const std::size_t n_1  = x.tape().row_size();
  const std::size_t nn_1 = x.value().tape().row_size();

  y = x * x * x;

  EXPECT_EQ(y, 4 * 4 * 4);

  auto y_gradient = Jacobian_row(y);

  EXPECT_EQ(y_gradient[x].value(), 3 * 4 * 4);

  auto Hessian_x_row = Jacobian_row(y_gradient[x]);

  EXPECT_EQ(Hessian_x_row[x.value()], 3 * 2 * 4);

  EXPECT_EQ(1, x.tape().row_size() - n_1);
  EXPECT_EQ(14, x.value().tape().row_size() - nn_1);
}

TEST(Nested, Random_1)
{
  AD<AD<double>> x1(1), x2(2), x3(3), y;

  y = 2 * x1 + x2 * x3 / (1 + x1 * x2 + x3);

  ASSERT_DOUBLE_EQ(y.value().value(), 3);

  auto y_gradient = Jacobian_row(y);

  ASSERT_DOUBLE_EQ(y_gradient[x1].value(), 5 / 3.);
  ASSERT_DOUBLE_EQ(y_gradient[x2].value(), 1 / 3.);
  ASSERT_DOUBLE_EQ(y_gradient[x3].value(), 1 / 6.);

  auto Hessian_row_x2 = Jacobian_row(y_gradient[x2]);

  ASSERT_DOUBLE_EQ(Hessian_row_x2[x1.value()], -2 / 9.);
  ASSERT_DOUBLE_EQ(Hessian_row_x2[x2.value()], -1 / 9.);
  ASSERT_DOUBLE_EQ(Hessian_row_x2[x3.value()], +1 / 12.);

  auto Hessian_row_x3 = Jacobian_row(y_gradient[x3]);

  ASSERT_DOUBLE_EQ(Hessian_row_x3[x1.value()], 0);
  ASSERT_DOUBLE_EQ(Hessian_row_x3[x2.value()], +1 / 12.);
  ASSERT_DOUBLE_EQ(Hessian_row_x3[x3.value()], -1 / 18.);
}
