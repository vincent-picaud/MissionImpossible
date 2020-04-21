#include "MissionImpossible/tape_vector.hpp"
#include "MissionImpossible/ad.hpp"
#include "MissionImpossible/mission_impossible_tape.hpp"
#include "MissionImpossible/tape.hpp"

#include <gtest/gtest.h>
#include <vector>

using namespace MissionImpossible;

TEST(Tape_Vector, demo)
{
  using T = double;

  [[maybe_unused]] AD<T> x;
  x = 1;

  EXPECT_EQ(tape<T>().statement_size(), 1);

  x = 2;

  EXPECT_EQ(tape<T>().statement_size(), 2);

  auto jb_tape = MissionImpossible_Tape<T>();

  EXPECT_EQ(tape<T>().statement_size(), 2);

  [[maybe_unused]] AD<T> y = T(1);
  //  y = 1;

  EXPECT_EQ(tape<T>().statement_size(), 3);

  // Tape_Vector<Tape<T>> tape_vector(jb_tape);

  // EXPECT_EQ(tape_vector.size(), 1);
}
