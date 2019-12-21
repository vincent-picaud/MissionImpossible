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

  y = 3 * x * x;
  std::cout << "\n tape before \n" << tape<T>();
  {
    auto mark = tape<T>().JamesBond_mark();

    y = x * y;
    std::cout << tape<T>();

    std::vector<T> diff(20, 0);
    diff[x.index()] = 1;

    tape<T>().forward(y.index(), diff.data());

    std::cout << "Diff forward\n" << diff;
  }

  std::cout << "\n tape after \n" << tape<T>();

  {
    auto mark = tape<T>().JamesBond_mark();

    y = x * y;
    std::cout << tape<T>();

    std::vector<T> diff(20, 0);
    diff[y.index()] = 1;

    tape<T>().reverse(y.index(), diff.data());

    std::cout << "Diff reverse\n" << diff;
  }
}
