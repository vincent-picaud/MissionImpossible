#include "AutoDiffCpp/mission_impossible_tape.hpp"
#include "AutoDiffCpp/ad.hpp"
#include "AutoDiffCpp/derivatives.hpp"

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

TEST(Mission_Impossible_Tape, local)
{
  using T = double;

  AD<T> x1(1), x2(2), y;

  y = 2 * x1 * x2;

  auto pos     = tape<T>().row_size();
  auto pos_mem = tape<T>().memory_size();

  {
    Mission_Impossible_Tape<T> self_destructing_tape;

    AD<T> a(1), b(2), c;

    c = 2 * a * b;

     auto grad = gradient(self_destructing_tape, c);
    // auto grad = gradient(c);

    EXPECT_EQ(grad.size(), 3);
  }

  EXPECT_EQ(pos, tape<T>().row_size());
  EXPECT_EQ(pos_mem, tape<T>().memory_size());
}
