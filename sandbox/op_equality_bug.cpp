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
  AD<AD<double>> x0(3);

  const std::size_t n_1  = x0.tape().row_size();
  const std::size_t nn_1 = x0.value().tape().row_size();
  const bool ok          = x0 == 5;  // this must not create an instance! NOT OK
  assert(ok);
  std::cout << "\n n_1 " << n_1 << " vs " << x0.tape().row_size();
  std::cout << "\n nn_1 " << nn_1 << " vs " << x0.value().tape().row_size();
}
