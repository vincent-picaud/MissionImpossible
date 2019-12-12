#include "AutoDiffCpp/tape.hpp"

#include <vector>

using namespace AutoDiffCpp;

int main()
{
  using T = double;

  Tape<T> tape;

  std::vector<offset_type> buffer_offset(50, offset_type());
  std::vector<T> buffer_partial(50, T());

  tape.add_row(1, buffer_offset.data(), buffer_partial.data());
  std::cout << tape;

  tape.add_row(10, buffer_offset.data(), buffer_partial.data());
  std::cout << tape;
}
