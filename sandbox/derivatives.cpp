#include "AutoDiffCpp/derivatives.hpp"

using namespace AutoDiffCpp;

int
main()
{
  AD<double> x(3), y;

  y = x * x;

  std::cout << y.tape() << std::endl;

  // auto grad = gradient(y);
   auto grad = Jacobian_column(y);

  std::cout << grad;
}
