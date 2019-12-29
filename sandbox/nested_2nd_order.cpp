#include "AutoDiffCpp/ad.hpp"
#include "AutoDiffCpp/derivatives.hpp"

#include <vector>

using namespace AutoDiffCpp;

int
main()
{
  AD<AD<double>> x0(3), x1(4), y;

  y = (1 - x0) * (1 - x0) + 10 * (x1 - x0 * x0) * (x1 - x0 * x0);

  assert(y == 254);

  auto y_gradient = Jacobian_row(y);

  assert(y_gradient[x0] == 604);
  assert(y_gradient[x1] == -100);

  auto Hessian_x0_row = Jacobian_row(y_gradient[x0]);

  assert(Hessian_x0_row[x0] == 922);
  assert(Hessian_x0_row[x1] == -120);

  auto Hessian_x1_row = Jacobian_row(y_gradient[x1]);

  assert(Hessian_x1_row[x0] == -120);
  assert(Hessian_x1_row[x1] == 20);

  // Check tape length
  //
  std::cout << "Tape<T> length     " << x0.value().tape().row_size() << std::endl;
  std::cout << "Tape<AD<T>> length " << x0.tape().row_size() << std::endl;

  std::cout << "Tape<T>\n" << x0.value().tape();
  std::cout << "Tape<AD<T>>\n" << x0.tape();
}
