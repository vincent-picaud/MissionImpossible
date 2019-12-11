#include "AutoDiffCpp/tape.hpp"

using namespace AutoDiffCpp;

int main()
{
  Tape<double> tape;

  auto* p = tape.add_row(1);
  std::cout << tape;

  p = tape.add_row(10);
  std::cout << tape;
}
