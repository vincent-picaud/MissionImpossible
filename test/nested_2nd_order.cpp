#include "MissionImpossible/ad.hpp"
#include "MissionImpossible/derivatives.hpp"

#include <gtest/gtest.h>
#include <vector>

using namespace MissionImpossible;

TEST(Nested, Rosenbrock)
{
  AD<AD<double>> x0(3), x1(4), y;

  y = (1 - x0) * (1 - x0) +
      10 * (x1 - x0 * x0) * (x1 - x0 * x0);  // remark: y[x0] type is  AD<AD<T>>

  EXPECT_EQ(y, 254);
  auto y_gradient = Jacobian_row(y);

  EXPECT_EQ(y_gradient[x0], 604);  // remark: y_gradient[x0] type is AD<T>
  EXPECT_EQ(y_gradient[x1], -100);

  auto Hessian_x0_row = Jacobian_row(y_gradient[x0]);  // remark: Hessian_x0_row[x0] type is T

  EXPECT_EQ(Hessian_x0_row[x0], 922);
  EXPECT_EQ(Hessian_x0_row[x1], -120);

  auto Hessian_x1_row = Jacobian_row(y_gradient[x1]);

  EXPECT_EQ(Hessian_x1_row[x0], -120);
  EXPECT_EQ(Hessian_x1_row[x1], 20);

  // Check tape length
  //
  EXPECT_EQ(x0.tape().row_size(), 3);
  EXPECT_EQ(x0.value().tape().row_size(), 39);

  EXPECT_EQ(x0.tape().memory_size(), 288);
  EXPECT_EQ(x0.value().tape().memory_size(), 1424);
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

  EXPECT_EQ(y_gradient[x], 3 * 4 * 4);

  auto Hessian_x_row = Jacobian_row(y_gradient[x]);

  EXPECT_EQ(Hessian_x_row[x], 3 * 2 * 4);

  EXPECT_EQ(1, x.tape().row_size() - n_1);
  EXPECT_EQ(17, x.value().tape().row_size() - nn_1);
}

// NOTE: when using ASSERT_DOUBLE_EQ instead of EXPECT_EQ, the
// overloaded ==(AD,AD) operator is not used anymore and one must
// provide a double. This force us to use the ugly syntax
// y.value().value().value()...
//
TEST(Nested, Random_1)
{
  AD<AD<double>> x1(1), x2(2), x3(3), y;

  y = 2 * x1 + x2 * x3 / (1 + x1 * x2 + x3);

  ASSERT_DOUBLE_EQ(y.value().value(), 3);

  auto y_gradient = Jacobian_row(y);

  ASSERT_DOUBLE_EQ(y_gradient[x1].value(), 5 / 3.);
  ASSERT_DOUBLE_EQ(y_gradient[x2].value(), 1 / 3.);
  ASSERT_DOUBLE_EQ(y_gradient[x3].value(), 1 / 6.);

  auto Hessian_row_x1 = Jacobian_row(y_gradient[x1]);

  ASSERT_DOUBLE_EQ(Hessian_row_x1[x1], +2 / 9.);
  ASSERT_DOUBLE_EQ(Hessian_row_x1[x2], -2 / 9.);
  ASSERT_DOUBLE_EQ(Hessian_row_x1[x3], 0.);

  auto Hessian_row_x2 = Jacobian_row(y_gradient[x2]);

  ASSERT_DOUBLE_EQ(Hessian_row_x2[x1], -2 / 9.);
  ASSERT_DOUBLE_EQ(Hessian_row_x2[x2], -1 / 9.);
  ASSERT_DOUBLE_EQ(Hessian_row_x2[x3], +1 / 12.);

  auto Hessian_row_x3 = Jacobian_row(y_gradient[x3]);

  ASSERT_DOUBLE_EQ(Hessian_row_x3[x1], 0);
  ASSERT_DOUBLE_EQ(Hessian_row_x3[x2], +1 / 12.);
  ASSERT_DOUBLE_EQ(Hessian_row_x3[x3], -1 / 18.);
}

TEST(Nested, test_3_order)
{
  AD<AD<AD<double>>> x1(1), x2(2), x3(3), y;

  const std::size_t n_1   = x1.tape().row_size();
  const std::size_t nn_1  = x1.value().tape().row_size();
  const std::size_t nnn_1 = x1.value().value().tape().row_size();

  y = 2 * x1 + x2 * x3 / (1 + x1 * x2 + x3);

  ASSERT_DOUBLE_EQ(y.value().value().value(), 3);

  auto y_gradient = Jacobian_row(y);

  ASSERT_DOUBLE_EQ(y_gradient[x1].value().value(), 5 / 3.);
  ASSERT_DOUBLE_EQ(y_gradient[x2].value().value(), 1 / 3.);
  ASSERT_DOUBLE_EQ(y_gradient[x3].value().value(), 1 / 6.);

  auto Hessian_row_x1 = Jacobian_row(y_gradient[x1]);

  ASSERT_DOUBLE_EQ(Hessian_row_x1[x1].value(), +2 / 9.);
  ASSERT_DOUBLE_EQ(Hessian_row_x1[x2].value(), -2 / 9.);
  ASSERT_DOUBLE_EQ(Hessian_row_x1[x3].value(), 0.);

  auto Hessian_row_x2 = Jacobian_row(y_gradient[x2]);

  ASSERT_DOUBLE_EQ(Hessian_row_x2[x1].value(), -2 / 9.);
  ASSERT_DOUBLE_EQ(Hessian_row_x2[x2].value(), -1 / 9.);
  ASSERT_DOUBLE_EQ(Hessian_row_x2[x3].value(), +1 / 12.);

  auto Hessian_row_x3 = Jacobian_row(y_gradient[x3]);

  ASSERT_DOUBLE_EQ(Hessian_row_x3[x1].value(), 0);
  ASSERT_DOUBLE_EQ(Hessian_row_x3[x2].value(), +1 / 12.);
  ASSERT_DOUBLE_EQ(Hessian_row_x3[x3].value(), -1 / 18.);

  //////////////////

  auto Hessian_row_x1_x1 = Jacobian_row(Hessian_row_x1[x1]);

  ASSERT_DOUBLE_EQ(Hessian_row_x1_x1[x1], -2 / 9.);
  ASSERT_DOUBLE_EQ(Hessian_row_x1_x1[x2], +2 / 9.);
  ASSERT_DOUBLE_EQ(Hessian_row_x1_x1[x3], -1 / 27.);

  auto Hessian_row_x1_x2 = Jacobian_row(Hessian_row_x1[x2]);

  ASSERT_DOUBLE_EQ(Hessian_row_x1_x2[x1], +2 / 9.);
  ASSERT_DOUBLE_EQ(Hessian_row_x1_x2[x2], 0.);
  ASSERT_DOUBLE_EQ(Hessian_row_x1_x2[x3], -1 / 54.);

  //
  EXPECT_EQ(x1.tape().row_size() - n_1, 1);
  EXPECT_EQ(x1.value().tape().row_size() - nn_1, 23);
  EXPECT_EQ(x1.value().value().tape().row_size() - nnn_1, 295);
}
