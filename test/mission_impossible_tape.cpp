#include "AutoDiffCpp/mission_impossible_tape.hpp"
#include "AutoDiffCpp/ad.hpp"

#include <gtest/gtest.h>

using namespace AutoDiffCpp;

TEST(Mission_Impossible_Tape, construction)
{
  Mission_Impossible_Tape<double> mark;

  EXPECT_EQ(mark.size(), 0);
}
TEST(Mission_Impossible_Tape, construction_nested)
{
  Mission_Impossible_Tape<AD<AD<double>>> mark;

  EXPECT_EQ(mark.size(), 0);
}
