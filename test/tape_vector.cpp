#include "Mission_Impossible_AutoDiff/tape_vector.hpp"
#include "Mission_Impossible_AutoDiff/ad.hpp"
#include "Mission_Impossible_AutoDiff/tape.hpp"
#include "Mission_Impossible_AutoDiff/mission_impossible_tape.hpp"

#include <gtest/gtest.h>
#include <vector>

using namespace Mission_Impossible_AutoDiff;

TEST(Tape_Vector, demo)
{
  using T = double;

  [[maybe_unused]] AD<T> x;
  x = 1;

  EXPECT_EQ(tape<T>().row_size(), 1);

  x = 2;

  EXPECT_EQ(tape<T>().row_size(), 2);

  auto jb_tape = Mission_Impossible_Tape<T>();

  EXPECT_EQ(tape<T>().row_size(), 2);

  [[maybe_unused]] AD<T> y = T(1);
  //  y = 1;

  EXPECT_EQ(tape<T>().row_size(), 3);

  // Tape_Vector<Tape<T>> tape_vector(jb_tape);

  // EXPECT_EQ(tape_vector.size(), 1);
}
