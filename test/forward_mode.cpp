#include "Mission_Impossible_AutoDiff/derivatives.hpp"
#include "Mission_Impossible_AutoDiff/functions.hpp"

#include <gtest/gtest.h>
#include <vector>

using namespace Mission_Impossible_AutoDiff;

TEST(Forward_mode, basic)
{
  AD<double> x1 = 2, x2 = 3, y1, y2;

  y1 = cos(x1 * x2);
  y2 = sin(x1 * x2);

  auto J_column_1 = Jacobian_column(x1);

  ASSERT_DOUBLE_EQ(J_column_1[y1], -x2.value() * sin(x1.value() * x2.value()));
  ASSERT_DOUBLE_EQ(J_column_1[y2], x2.value() * cos(x1.value() * x2.value()));

  auto J_column_2 = Jacobian_column(x2);

  ASSERT_DOUBLE_EQ(J_column_2[y1], -x1.value() * sin(x1.value() * x2.value()));
  ASSERT_DOUBLE_EQ(J_column_2[y2], x1.value() * cos(x1.value() * x2.value()));
}

TEST(Forward_mode, basic_local)
{
  Mission_Impossible_Tape<double> local_tape;

  AD<double> x1 = 2, x2 = 3, y1, y2;

  y1 = cos(x1 * x2);
  y2 = sin(x1 * x2);

  auto J_column_1 = Jacobian_column(local_tape, x1);

  ASSERT_DOUBLE_EQ(J_column_1[y1], -x2.value() * sin(x1.value() * x2.value()));
  ASSERT_DOUBLE_EQ(J_column_1[y2], x2.value() * cos(x1.value() * x2.value()));

  auto J_column_2 = Jacobian_column(local_tape, x2);

  ASSERT_DOUBLE_EQ(J_column_2[y1], -x1.value() * sin(x1.value() * x2.value()));
  ASSERT_DOUBLE_EQ(J_column_2[y2], x1.value() * cos(x1.value() * x2.value()));
}
