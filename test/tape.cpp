#include "AutoDiffCpp/tape.hpp"
#include <gtest/gtest.h>

using namespace AutoDiffCpp;

TEST(Tape, demo)
{
  Tape<double> tape;

  ASSERT_TRUE(tape.tape_size() > 0);
  ASSERT_TRUE(tape.tape_capacity() > 0);
}
