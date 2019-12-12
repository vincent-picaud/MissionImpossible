#include "AutoDiffCpp/tape.hpp"

#include <gtest/gtest.h>
#include <vector>

using namespace AutoDiffCpp;

TEST(Tape, demo)
{
  Tape<double> tape;

  ASSERT_EQ(tape.row_size(), 0);

  std::vector<Index_PartialDiff<double>> buffer(50, Index_PartialDiff<double>());
  tape.add_row(1, buffer.data());
  ASSERT_EQ(tape.row_size(), 1);

  tape.add_row(10, buffer.data());
  ASSERT_EQ(tape.row_size(), 2);
}
