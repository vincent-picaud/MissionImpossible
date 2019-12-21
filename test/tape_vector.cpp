#include "AutoDiffCpp/tape_vector.hpp"
#include "AutoDiffCpp/ad.hpp"
#include "AutoDiffCpp/tape.hpp"

#include <gtest/gtest.h>
#include <vector>

using namespace AutoDiffCpp;

TEST(Tape_Vector, demo)
{
  using T = double;

  [[maybe_unused]] AD<T> x;
  x = 1;

  EXPECT_EQ(tape<T>().row_size(), 1);

  x = 2;

  EXPECT_EQ(tape<T>().row_size(), 2);

  auto jb_tape = tape<T>().JamesBond_mark();

  EXPECT_EQ(tape<T>().row_size(), 2);

  [[maybe_unused]] AD<T> y = T(1);
  //  y = 1;

  EXPECT_EQ(tape<T>().row_size(), 3);

  // Tape_Vector<Tape<T>> tape_vector(jb_tape);

  // EXPECT_EQ(tape_vector.size(), 1);
}
