#include "AutoDiffCpp/ad.hpp"

#include <vector>

using namespace AutoDiffCpp;

int
main()
{
  using T = double;

  AD<T> y, x(2);

  y = x * x;

  y = x * y;
  std::cout << y.tape();
}
