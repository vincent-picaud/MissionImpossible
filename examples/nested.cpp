#include "MissionImpossible/MissionImpossible.hpp"

#include <iostream>

using namespace MissionImpossible;

template <typename T>
auto
Rosenbrock(const T& x0, const T& x1)
{
  return (1 - x0) * (1 - x0) + 10 * (x1 - x0 * x0) * (x1 - x0 * x0);
}

// Third order demo
int
main()
{
  AD<AD<AD<double>>> x0(3), x1(4), y;

  y = Rosenbrock(x0, x1);

  auto grad = gradient(y);

  auto Hessian_x0_row = gradient(grad[x0]);
  auto Hessian_x1_row = gradient(grad[x1]);

  auto third_order_x0_x0_row = gradient(Hessian_x0_row[x0]);
  auto third_order_x0_x1_row = gradient(Hessian_x0_row[x1]);
  auto third_order_x1_x0_row = gradient(Hessian_x1_row[x0]);
  auto third_order_x1_x1_row = gradient(Hessian_x1_row[x1]);

  std::cout << "f     = " << y << std::endl;
  std::cout << std::endl;
  std::cout << "∂₀f   = " << grad[x0] << std::endl;
  std::cout << "∂₁f   = " << grad[x1] << std::endl;
  std::cout << std::endl;
  std::cout << "∂²₀₀f = " << Hessian_x0_row[x0] << std::endl;
  std::cout << "∂²₀₁f = " << Hessian_x0_row[x1] << std::endl;
  std::cout << "∂²₁₀f = " << Hessian_x1_row[x0] << std::endl;
  std::cout << "∂²₁₁f = " << Hessian_x1_row[x1] << std::endl;
  std::cout << std::endl;
  std::cout << "∂³₀₀₀f = " << third_order_x0_x0_row[x0] << std::endl;
  std::cout << "∂³₀₀₁f = " << third_order_x0_x0_row[x1] << std::endl;
  std::cout << "∂³₀₁₀f = " << third_order_x0_x1_row[x0] << std::endl;
  std::cout << "∂³₀₁₁f = " << third_order_x0_x1_row[x1] << std::endl;
  std::cout << "∂³₁₀₀f = " << third_order_x1_x0_row[x0] << std::endl;
  std::cout << "∂³₁₀₁f = " << third_order_x1_x0_row[x1] << std::endl;
  std::cout << "∂³₁₁₀f = " << third_order_x1_x1_row[x0] << std::endl;
  std::cout << "∂³₁₁₁f = " << third_order_x1_x1_row[x1] << std::endl;
}
