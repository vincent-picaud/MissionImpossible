#include "AutoDiffCpp/tape.hpp"

#include <vector>

using namespace AutoDiffCpp;

int
main()
{
  using T = double;

  Tape<T> tape;
  using offset_type = typename Tape<T>::offset_type;
  using value_type  = typename Tape<T>::value_type;

  std::vector<offset_type> buffer_offset(50, offset_type());
  std::vector<value_type> buffer_partial(50, T());

  tape.add_row(1, buffer_offset.data(), buffer_partial.data());
  std::cout << tape;

  tape.add_row(10, buffer_offset.data(), buffer_partial.data());
  std::cout << tape;
}
