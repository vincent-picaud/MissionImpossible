#include "AutoDiffCpp/ad.hpp"

#include <iomanip>
#include <vector>

using namespace AutoDiffCpp;

template <typename T>
std::ostream&
operator<<(std::ostream& out, const std::vector<T>& to_print)
{
  for (std::size_t i = 0; i < to_print.size(); ++i)
  {
    out << std::setw(4) << i << " " << std::setw(16) << to_print[i] << std::endl;
  }
  return out;
}

int
main()
{
  using T = double;

  AD<T> y, x(2);

  y = x * x;

  y = x * y;
  std::cout << y.tape();

  std::vector<T> diff(20, 0);
  diff[x.index()[0]] = 1;

  y.tape().forward(y.index()[0], diff.data());

  std::cout << "Diff\n" << diff;
}
