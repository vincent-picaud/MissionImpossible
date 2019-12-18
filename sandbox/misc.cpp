// This show that in case of nested type, y gradient computation
// add element to the tape
//
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

  y = x0 + 2 * x1;

  std::cout << "Tape memory " << x0.tape().memory_size() << "\t" << x0.value().tape().memory_size()
            << std::endl;

  std::cout << "Tape AD<AD>\n" << x0.tape() << std::endl;
  std::cout << "Tape AD<T>\n" << x0.value().tape() << std::endl;

  // NOTE: this increase the AD<T> tape
  //       -> this is normal, as we also compute second order derivatives
  //
  auto grad_y = gradient(y);

  std::cout << "grad_y\n" << grad_y;

  std::cout << "Tape memory " << x0.tape().memory_size() << "\t" << x0.value().tape().memory_size()
            << std::endl;

  std::cout << "Tape AD<AD>\n" << x0.tape() << std::endl;
  std::cout << "Tape AD<T>\n" << x0.value().tape() << std::endl;
}
