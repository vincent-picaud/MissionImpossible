#include "MissionImpossible/MissionImpossible.hpp"

using namespace MissionImpossible;

template <typename T>
T
my_function(const AD_Underlying_Type_t<T> scalar_constant, const T x)
{
  return scalar_constant * x * x;
}

int
main()
{
  AD<AD<double>> x = 2, y;

  y = my_function(10, x);

  auto dy  = Jacobian_row(y);      // auto = Tape_Vector<AD<double>>
  auto d2y = Jacobian_row(dy[x]);  // auto = Tape_Vector<double>

  std::cout << "y   = " << y << std::endl;
  std::cout << "dy  = " << dy[x] << " dx" << std::endl;
  std::cout << "d2y = " << d2y[x] << " dx⊗dx" << std::endl;
}
