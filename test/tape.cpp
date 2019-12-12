#include "AutoDiffCpp/tape.hpp"

#include <gtest/gtest.h>
#include <vector>

using namespace AutoDiffCpp;

TEST(Tape, demo)
{
  Tape<double> tape;

  ASSERT_EQ(tape.row_size(), 0);

  std::vector<offset_type> buffer_offset(50, offset_type());
  std::vector<double> buffer_partial(50, double());

  tape.add_row(1, buffer_offset.data(), buffer_partial.data());
  ASSERT_EQ(tape.row_size(), 1);

  tape.add_row(10, buffer_offset.data(), buffer_partial.data());
  ASSERT_EQ(tape.row_size(), 2);
}
