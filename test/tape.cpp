#include "MissionImpossible/tape.hpp"

#include <gtest/gtest.h>
#include <vector>

using namespace MissionImpossible;

TEST(Tape, demo)
{
  using tape_type  = Tape<double>;
  using index_type = typename tape_type::index_type;
  using value_type = typename tape_type::value_type;

  tape_type tape;

  ASSERT_EQ(tape.statement_size(), 0);

  std::vector<index_type> buffer_index(50, index_type());
  std::vector<value_type> buffer_partial(50, double());

  tape.add_statement(1, buffer_index.data(), buffer_partial.data());
  ASSERT_EQ(tape.statement_size(), 1);

  tape.add_statement(10, buffer_index.data(), buffer_partial.data());
  ASSERT_EQ(tape.statement_size(), 2);
}
