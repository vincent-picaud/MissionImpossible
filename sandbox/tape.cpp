#include "AutoDiffCpp/tape.hpp"

#include <vector>

using namespace AutoDiffCpp;

int main()
{
  using T = double;

  Tape<T> tape;

  std::vector<Index_PartialDiff<T>> buffer(50);

  tape.add_row(1, buffer.data());
  std::cout << tape;

  tape.add_row(10, buffer.data());
  std::cout << tape;
}
