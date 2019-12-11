#include "AutoDiffCpp/tape.hpp"

#include <vector>
#include <gtest/gtest.h>

using namespace AutoDiffCpp;

TEST(Tape, demo)
{
  Tape<double> tape;

  ASSERT_EQ(tape.row_size(), 0);

  std::vector<double> buffer(50);
  tape.add_row(1,buffer.data());
  ASSERT_EQ(tape.row_size(), 1);

  p = tape.add_row(10,buffer);
  ASSERT_EQ(tape.row_size(), 2);
}
