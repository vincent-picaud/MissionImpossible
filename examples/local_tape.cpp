#include "MissionImpossible/MissionImpossible.hpp"

#include <array>
#include <iostream>

using namespace MissionImpossible;

// A C++ function
template <typename T>
T
Rosenbrock(const std::array<T, 2>& X)
{
  return (1 - X[0]) * (1 - X[0]) + 10 * (X[1] - X[0] * X[0]) * (X[1] - X[0] * X[0]);
}

// A C++ function that adds gradient computation
template <typename T>
T
Rosenbrock(const std::array<T, 2>& X, std::array<T, 2>& grad)
{
  MissionImpossible_Tape<T> local_tape;  // a local thread_local tape

  std::array<AD<T>, 2> ad_X;

  ad_X[0] = X[0];
  ad_X[1] = X[1];

  AD<T> ad_f = Rosenbrock(ad_X);

  Tape_Vector<T> ad_grad_f = gradient(local_tape, ad_f);  // use the local tape for ∇f

  grad[0] = ad_grad_f[ad_X[0]];
  grad[1] = ad_grad_f[ad_X[1]];

  return ad_f.value();

  // here the local tape is destroyed (in fact re-winded to avoid
  // useless new/delete)
}

int
main()
{
  std::array<float, 2> X{3., 4.};

  float f1 = Rosenbrock(X);

  std::cout << "f1  = " << f1 << std::endl;

  std::array<float, 2> grad;

  float f2 = Rosenbrock(X, grad);

  std::cout << "f2  = " << f2 << std::endl;
  std::cout << "∇f2 = [ " << grad[0] << ", " << grad[1] << " ]" << std::endl;
}
