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

// void
// more_versatile()
// {
//   using T = std::complex<AD<double>>;

//   T z0 = T(1, 2), Z;

//   Z = 4 * exp(2 * z0 * z0);

//   auto dZ = gradient(Z.real());  // assumes that Z is holomorph

//   std::cout << " f = " << Z << std::endl;
//   std::cout << "df = " << dZ[z0.real()] << ", ";
//   std::cout << dZ[z0.imag()] << std::endl;

//   // Cauchy-Riemann
//   //
//   const auto I = std::complex<double>(0, 1);

//   auto tmp = gradient(Z.imag());

//   std::cout << I * dZ[z0.real()] << " ?= " << tmp[z0.real()] << std::endl;
//   std::cout << I * dZ[z0.imag()] << " ?= " << tmp[z0.imag()] << std::endl;
// }

void
more_versatile()
{
  AD<double> x(1), y(2);
  std::complex<AD<double>> z0(x, y), Z;

  Z = 4 * exp(2 * z0 * z0);

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

  std::cout << grad_u[x] << " ?= " << grad_v[y] << std::endl;
  std::cout << grad_u[y] << " ?= " << -grad_v[x] << std::endl;
}

void
more_versatile_2()
{
  AD<double> x(1), y(2);
  std::complex<AD<double>> z0(x, y), Z;

  Z = sqrt(z0 * conj(z0));

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

  std::cout << grad_u[x] << " ?= " << grad_v[y] << std::endl;
  std::cout << grad_u[y] << " ?= " << -grad_v[x] << std::endl;
}
int
main()
{
  most_efficient();
  more_versatile();
  more_versatile_2();
}
