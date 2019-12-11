#include "AutoDiffCpp/tape.hpp"
#include <gtest/gtest.h>

using namespace AutoDiffCpp;

TEST(Tape, demo)
{
  Tape<double> tape;

  ASSERT_EQ(tape.row_size(), 0);

  auto* p = tape.add_row(1);
  ASSERT_EQ(tape.row_size(), 1);

  p = tape.add_row(10);
  ASSERT_EQ(tape.row_size(), 2);
}
