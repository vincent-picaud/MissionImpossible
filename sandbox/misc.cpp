// Investigate why
//
// AD<AD<double>> x0(3);
// x0 == 3;
//
// create an instance
#include "AutoDiffCpp/ad.hpp"
#include "AutoDiffCpp/derivatives.hpp"

#include <vector>

using namespace AutoDiffCpp;

int
main()
{
  AD<AD<double>> x0(3), x1(4), y;

  std::cout << "Tape memory " << x0.tape().memory_size() << "\t" << x0.value().tape().memory_size()
            << std::endl;

  y = x0 + x1;

  std::cout << "Tape memory " << x0.tape().memory_size() << "\t" << x0.value().tape().memory_size()
            << std::endl;
}
