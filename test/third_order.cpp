#include "MissionImpossible/MissionImpossible.hpp"

#include <gtest/gtest.h>

using namespace MissionImpossible;

// Check 3 order computation
//
TEST(Third_Order, Rosenbrock)
{
  using T = double;

  std::size_t mem_0_size = tape<T>().memory_size();
  std::size_t mem_1_size = tape<AD<T>>().memory_size();
  std::size_t mem_2_size = tape<AD<AD<T>>>().memory_size();
  std::size_t mem_3_size = tape<AD<AD<AD<T>>>>().memory_size();

  AD<AD<AD<T>>> x0(3), x1(4), y;

  y = (1 - x0) * (1 - x0) + 10 * (x1 - x0 * x0) * (x1 - x0 * x0);

  auto grad = gradient(y);

  auto Hessian_x0_row = gradient(grad[x0]);
  auto Hessian_x1_row = gradient(grad[x1]);

  auto third_order_x0_x0_row = gradient(Hessian_x0_row[x0]);
  auto third_order_x0_x1_row = gradient(Hessian_x0_row[x1]);
  auto third_order_x1_x0_row = gradient(Hessian_x1_row[x0]);
  auto third_order_x1_x1_row = gradient(Hessian_x1_row[x1]);

  EXPECT_EQ(y, 254);

  EXPECT_EQ(grad[x0], 604);
  EXPECT_EQ(grad[x1], -100);

  EXPECT_EQ(Hessian_x0_row[x0], +922);
  EXPECT_EQ(Hessian_x0_row[x1], -120);
  EXPECT_EQ(Hessian_x1_row[x0], -120);
  EXPECT_EQ(Hessian_x1_row[x1], 20);

  EXPECT_EQ(third_order_x0_x0_row[x0], 720);
  EXPECT_EQ(third_order_x0_x0_row[x1], -40);
  EXPECT_EQ(third_order_x0_x1_row[x0], -40);
  EXPECT_EQ(third_order_x0_x1_row[x1], 0);
  EXPECT_EQ(third_order_x1_x0_row[x0], -40);
  EXPECT_EQ(third_order_x1_x0_row[x1], 0);
  EXPECT_EQ(third_order_x1_x1_row[x0], 0);
  EXPECT_EQ(third_order_x1_x1_row[x1], 0);

  // Check memory consumption
  //
  mem_0_size = tape<T>().memory_size() - mem_0_size;
  mem_1_size = tape<AD<T>>().memory_size() - mem_1_size;
  mem_2_size = tape<AD<AD<T>>>().memory_size() - mem_2_size;
  mem_3_size = tape<AD<AD<AD<T>>>>().memory_size() - mem_3_size;

  EXPECT_EQ(mem_0_size, 7816);
  EXPECT_EQ(mem_1_size, 2520);
  EXPECT_EQ(mem_2_size, 536);
  EXPECT_EQ(mem_3_size, 0);
}

// Checks that local_tape correctly free used memory
//
TEST(Third_Order, Rosenbrock_with_local_tape)
{
  using T = double;

  std::size_t mem_0_size = tape<T>().memory_size();
  std::size_t mem_1_size = tape<AD<T>>().memory_size();
  std::size_t mem_2_size = tape<AD<AD<T>>>().memory_size();
  std::size_t mem_3_size = tape<AD<AD<AD<T>>>>().memory_size();

  {
    MissionImpossible_Tape<AD<AD<T>>> local_tape;
    AD<AD<AD<T>>> x0(3), x1(4), y;

    y = (1 - x0) * (1 - x0) + 10 * (x1 - x0 * x0) * (x1 - x0 * x0);

    auto grad = gradient(y);

    auto Hessian_x0_row = gradient(grad[x0]);
    auto Hessian_x1_row = gradient(grad[x1]);

    auto third_order_x0_x0_row = gradient(Hessian_x0_row[x0]);
    auto third_order_x0_x1_row = gradient(Hessian_x0_row[x1]);
    auto third_order_x1_x0_row = gradient(Hessian_x1_row[x0]);
    auto third_order_x1_x1_row = gradient(Hessian_x1_row[x1]);

    EXPECT_EQ(y, 254);

    EXPECT_EQ(grad[x0], 604);
    EXPECT_EQ(grad[x1], -100);

    EXPECT_EQ(Hessian_x0_row[x0], +922);
    EXPECT_EQ(Hessian_x0_row[x1], -120);
    EXPECT_EQ(Hessian_x1_row[x0], -120);
    EXPECT_EQ(Hessian_x1_row[x1], 20);

    EXPECT_EQ(third_order_x0_x0_row[x0], 720);
    EXPECT_EQ(third_order_x0_x0_row[x1], -40);
    EXPECT_EQ(third_order_x0_x1_row[x0], -40);
    EXPECT_EQ(third_order_x0_x1_row[x1], 0);
    EXPECT_EQ(third_order_x1_x0_row[x0], -40);
    EXPECT_EQ(third_order_x1_x0_row[x1], 0);
    EXPECT_EQ(third_order_x1_x1_row[x0], 0);
    EXPECT_EQ(third_order_x1_x1_row[x1], 0);
  }

  // Check memory consumption
  //
  mem_0_size = tape<T>().memory_size() - mem_0_size;
  mem_1_size = tape<AD<T>>().memory_size() - mem_1_size;
  mem_2_size = tape<AD<AD<T>>>().memory_size() - mem_2_size;
  mem_3_size = tape<AD<AD<AD<T>>>>().memory_size() - mem_3_size;

  EXPECT_EQ(mem_0_size, 0);
  EXPECT_EQ(mem_1_size, 0);
  EXPECT_EQ(mem_2_size, 0);
  EXPECT_EQ(mem_3_size, 0);
}
