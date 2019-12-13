#include "AutoDiffCpp/tape.hpp"

#include <vector>

using namespace AutoDiffCpp;

int
main()
{
  using T = double;

  Tape<T> tape;
  using index_type = typename Tape<T>::index_type;
  using value_type = typename Tape<T>::value_type;

  std::vector<index_type> buffer_index(50, index_type());
  std::vector<value_type> buffer_partial(50, T());

  tape.add_row(1, buffer_index.data(), buffer_partial.data());
  std::cout << tape;

  tape.add_row(10, buffer_index.data(), buffer_partial.data());
  std::cout << tape;
}
