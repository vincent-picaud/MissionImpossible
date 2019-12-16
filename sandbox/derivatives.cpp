#include "AutoDiffCpp/derivatives.hpp"

using namespace AutoDiffCpp;

int
main()
{
  AD<double> x0(3), x1(4), y;

  y = (1 - x0 * x0) * (1 - x0 * x0) + 10 * (x1 - x0 * x0) * (x1 - x0 * x0);

  std::cout << y.tape() << std::endl;

  // auto grad = gradient(y);
  std::cout << "f value        \n" << y.value();
  std::cout << "Jacobian_row   \n" << Jacobian_row(y);
  std::cout << "Jacobian_column\n" << Jacobian_column(x0);
  std::cout << "Jacobian_column\n" << Jacobian_column(x1);
}
