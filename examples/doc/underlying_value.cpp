#include "MissionImpossible/MissionImpossible.hpp"

using namespace MissionImpossible;

int
main()
{
  AD<AD<double>> x = 2, y;

  y = 10 * x * x;

  auto dy  = Jacobian_row(y);      // auto = Tape_Vector<AD<double>>
  auto d2y = Jacobian_row(dy[x]);  // auto = Tape_Vector<double>

  double value_y   = underlying_value(y);
  double value_dy  = underlying_value(dy[x]);
  double value_d2y = underlying_value(d2y[x]);

  std::cout << "y   = " << value_y << std::endl;
  std::cout << "dy  = " << value_dy << " dx" << std::endl;
  std::cout << "d2y = " << value_d2y << " dx⊗dx" << std::endl;
}
