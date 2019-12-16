#include "AutoDiffCpp/derivatives.hpp"

using namespace AutoDiffCpp;

int
main()
{
  AD<double> x(3), y;

  y = x * x;

  std::cout << y.tape() << std::endl;

  // auto grad = gradient(y);
  std::cout << "Jacobian_row   \n" << Jacobian_row(y);
  std::cout << "Jacobian_column\n" << Jacobian_column(x);
}
