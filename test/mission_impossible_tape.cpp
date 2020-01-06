#include "MissionImpossible/mission_impossible_tape.hpp"
#include "MissionImpossible/ad.hpp"
#include "MissionImpossible/derivatives.hpp"

#include <gtest/gtest.h>

using namespace MissionImpossible;

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

    // c = 2 * a * b;
    // dc = 2 a db + 2 b da
    EXPECT_EQ(grad[b], 2 * a.value());
    EXPECT_EQ(grad[a], 2 * b.value());
  }

  EXPECT_EQ(pos, tape<T>().row_size());
  EXPECT_EQ(pos_mem, tape<T>().memory_size());
}

TEST(Mission_Impossible_Tape, local_nested)
{
  using T = AD<double>;

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

    // c = 2 * a * b;
    // dc = 2 a db + 2 b da
    EXPECT_EQ(grad[b], 2 * 1);
    EXPECT_EQ(grad[a], 2 * 2);
    // d^2c = 2  da db + 2 db da

    const auto H_row_a = gradient(self_destructing_tape, grad[a]);
    // auto H_row_a = gradient(grad[a]);
    EXPECT_EQ(H_row_a[a], 0);
    EXPECT_EQ(H_row_a[b], 2);
  }

  EXPECT_EQ(pos, tape<T>().row_size());
  EXPECT_EQ(pos_mem, tape<T>().memory_size());
}

TEST(Mission_Impossible_Tape, local_nested_nested)
{
  // CAVEAT: AD<AD<double>> for AD<AD<AD<double>>> variable
  Mission_Impossible_Tape<AD<AD<double>>> self_destructing_tape;

  AD<AD<AD<double>>> x1(1), x2(2), y;

  y = 2 * x1 * x2;

  auto grad   = gradient(self_destructing_tape, y);
  auto grad_2 = gradient(self_destructing_tape, grad[y]);
  auto grad_3 = gradient(self_destructing_tape, grad_2[y]);
}
