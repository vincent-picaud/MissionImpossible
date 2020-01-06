#include "Mission_Impossible_AutoDiff/ad.hpp"
#include "Mission_Impossible_AutoDiff/derivatives.hpp"

#include <vector>

using namespace Mission_Impossible_AutoDiff;

template <typename T>
auto
Rosenbrock(const T& x0, const T& x1)
{
  return (1 - x0) * (1 - x0) + 10 * (x1 - x0 * x0) * (x1 - x0 * x0);
}

void
directional()
{
  AD<AD<double>> x0(3), x1(4), y;

  y = Rosenbrock(x0, x1);

  assert(y == 254);

  auto y_gradient = Jacobian_row(y);

  assert(y_gradient[x0] == 604);
  assert(y_gradient[x1] == -100);

  AD<double> z;
  double v0(5), v1(6);

  z = v0 * y_gradient[x0] + v1 * y_gradient[x1];

  auto z_gradient = Jacobian_row(z);

  // auto Hessian_x0_column = Jacobian_column(y_gradient[x1]);

  // // assert(Hessian_x0_column[x0] == 922);
  // // assert(Hessian_x0_column[x1] == -120);

  std::cerr << z_gradient[x0] << std::endl;
  std::cerr << z_gradient[x1] << std::endl;

  std::cout << "Tape<T>\n" << x0.value().tape();
  std::cout << "Tape<AD<T>>\n" << x0.tape();
}

int
main()
{
  directional();
  return 0;

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
