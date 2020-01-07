#include "MissionImpossible/MissionImpossible.hpp"

using namespace MissionImpossible;

int
main()
{
  AD<AD<double>> x0(3), x1(4), y;

  y = (1 - x0) * (1 - x0) + 10 * (x1 - x0 * x0) * (x1 - x0 * x0);

  std::cout << "f = " << y << std::endl;

  auto y_gradient = gradient(y);  // Computes ∇f

  std::cout << "∇f= " << y_gradient[x0] << ", ";
  std::cout << y_gradient[x1] << std::endl;

  AD<double> z;

  double v0(5), v1(6);

  z = v0 * y_gradient[x0] + v1 * y_gradient[x1];  // Computes z=<∇f,v>

  auto z_gradient = gradient(z);  // Computes Hv = ∇z = ∇ <∇f,v>

  std::cout << "Hv= " << z_gradient[x0] << ", ";
  std::cout << z_gradient[x1] << std::endl;
}
