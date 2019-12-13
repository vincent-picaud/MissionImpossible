#include "AutoDiffCpp/tape.hpp"

#include <gtest/gtest.h>
#include <vector>

using namespace AutoDiffCpp;

TEST(Tape, demo)
{
  using tape_type   = Tape<double>;
  using offset_type = typename tape_type::offset_type;
  using value_type  = typename tape_type::value_type;

  tape_type tape;

  ASSERT_EQ(tape.row_size(), 0);

  std::vector<offset_type> buffer_offset(50, offset_type());
  std::vector<value_type> buffer_partial(50, double());

  tape.add_row(1, buffer_offset.data(), buffer_partial.data());
  ASSERT_EQ(tape.row_size(), 1);

  tape.add_row(10, buffer_offset.data(), buffer_partial.data());
  ASSERT_EQ(tape.row_size(), 2);
}
