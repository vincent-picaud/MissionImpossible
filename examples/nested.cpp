#include "Mission_Impossible_AutoDiff/Mission_Impossible.hpp"

#include <iostream>

using namespace Mission_Impossible_AutoDiff;

template <typename T>
auto
Rosenbrock(const T& x0, const T& x1)
{
  return (1 - x0) * (1 - x0) + 10 * (x1 - x0 * x0) * (x1 - x0 * x0);
}

// Gradient
//
void
gradient_example()
{
  std::cout << std::endl << __PRETTY_FUNCTION__ << std::endl;

  AD<double> x0(3), x1(4), y;

  y = Rosenbrock(x0, x1);

  auto grad = gradient(y);

  std::cout << "f     = " << y << std::endl << std::endl;
  std::cout << "∂₀f   = " << grad[x0] << std::endl;
  std::cout << "∂₁f   = " << grad[x1] << std::endl;
}

// Hessian
//
void
Hessian_example()
{
  std::cout << std::endl << __PRETTY_FUNCTION__ << std::endl;

  AD<AD<double>> x0(3), x1(4), y;

  y = Rosenbrock(x0, x1);

  auto grad           = gradient(y);
  auto Hessian_x0_row = gradient(grad[x0]);
  auto Hessian_x1_row = gradient(grad[x1]);

  std::cout << "f     = " << y << std::endl << std::endl;
  std::cout << "∂₀f   = " << grad[x0] << std::endl;
  std::cout << "∂₁f   = " << grad[x1] << std::endl << std::endl;
  std::cout << "∂²₀₀f = " << Hessian_x0_row[x0] << std::endl;
  std::cout << "∂²₀₁f = " << Hessian_x0_row[x1] << std::endl;
  std::cout << "∂²₁₀f = " << Hessian_x1_row[x0] << "\t" << Hessian_x1_row[x1] << std::endl;
  std::cout << "∂²₁₁f = " << Hessian_x1_row[x1] << std::endl;
}
int
main()
{
  gradient_example();
  Hessian_example();

  // AD<AD<double>> x0(3), x1(4), y;

  // y = (1 - x0) * (1 - x0) + 10 * (x1 - x0 * x0) * (x1 - x0 * x0);

  // assert(y == 254);

  // auto y_gradient = Jacobian_row(y);

  // assert(y_gradient[x0] == 604);
  // assert(y_gradient[x1] == -100);

  // auto Hessian_x0_row = Jacobian_row(y_gradient[x0]);

  // assert(Hessian_x0_row[x0] == 922);
  // assert(Hessian_x0_row[x1] == -120);

  // auto Hessian_x1_row = Jacobian_row(y_gradient[x1]);

  // assert(Hessian_x1_row[x0] == -120);
  // assert(Hessian_x1_row[x1] == 20);

  // // Check tape length
  // //
  // std::cout << "Tape<T> length     " << x0.value().tape().row_size() << std::endl;
  // std::cout << "Tape<AD<T>> length " << x0.tape().row_size() << std::endl;

  // std::cout << "Tape<T>\n" << x0.value().tape();
  // std::cout << "Tape<AD<T>>\n" << x0.tape();
}
