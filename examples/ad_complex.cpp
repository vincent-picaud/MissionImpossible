#include "MissionImpossible/MissionImpossible.hpp"

#include <complex>
#include <iostream>

using namespace MissionImpossible;

void
most_efficient()
{
  using T = std::complex<double>;

  AD<T> z0 = T(1, 2), Z;

  Z = 4 * exp(2 * z0 * z0);

  auto dZ = gradient(Z);

  std::cout << " f = " << Z << std::endl;
  std::cout << "df = " << dZ[z0] << std::endl;
}

template <typename F>
void
more_versatile(F f)
{
  AD<double> x(1), y(2);
  std::complex<AD<double>> z0(x, y), Z;

  Z = f(z0);

  AD<double> u = Z.real(), v = Z.imag();

  const auto grad_u = gradient(u);

  // assumes that Z is holomorph
  //
  std::cout << " f = " << Z << std::endl;
  std::cout << "df = " << grad_u[x] << ", ";
  std::cout << -grad_u[y] << std::endl;

  // Cauchy-Riemann
  //
  const auto grad_v = gradient(v);

  std::cout << "--> Cauchy-Riemann check:" << std::endl;
  std::cout << grad_u[x] << " ?= " << grad_v[y] << std::endl;
  std::cout << grad_u[y] << " ?= " << -grad_v[x] << std::endl;
}

int
main()
{
  std::cout << "          f1:   " << std::endl;
  most_efficient();

  //================

  auto f_holomorph     = [](const auto& z) { return 4 * exp(2 * z * z); };
  auto f_not_holomorph = [](const auto& z) { return sqrt(z * conj(z)); };

  std::cout << std::endl << "Holomorph f1:   " << std::endl;
  more_versatile(f_holomorph);

  std::cout << std::endl << "Not holomorph f2: " << std::endl;
  more_versatile(f_not_holomorph);
}
