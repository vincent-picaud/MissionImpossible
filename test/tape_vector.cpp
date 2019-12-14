#include "AutoDiffCpp/tape_vector.hpp"
#include "AutoDiffCpp/JamesBond_tape.hpp"
#include "AutoDiffCpp/ad.hpp"
#include "AutoDiffCpp/tape.hpp"

#include <gtest/gtest.h>
#include <vector>

using namespace AutoDiffCpp;

TEST(Tape_Vector, demo)
{
  using T = float;

  [[maybe_unused]] AD<T> x = 1;

  EXPECT_EQ(tape<T>().row_size(), 1);

  auto jb_tape = tape<T>().JamesBond_tape();

  EXPECT_EQ(tape<T>().row_size(), 1);

  [[maybe_unused]] AD<T> y = 1;

  EXPECT_EQ(tape<T>().row_size(), 2);

  Tape_Vector<Tape<T>> tape_vector(jb_tape);

  EXPECT_EQ(tape_vector.size(), 1);
}
