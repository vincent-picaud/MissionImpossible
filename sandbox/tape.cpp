#include "Mission_Impossible_AutoDiff/ad.hpp"
#include "Mission_Impossible_AutoDiff/mission_impossible_tape.hpp"

#include <iomanip>
#include <vector>

using namespace Mission_Impossible_AutoDiff;

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
    Mission_Impossible_Tape<T> mark;

    y = x * y;
    std::cout << tape<T>();

    std::vector<T> diff(20, 0);
    diff[x.index()] = 1;

    tape<T>().forward(y.index(), diff.data());

    std::cout << "Diff forward\n" << diff;
  }

  std::cout << "\n tape after \n" << tape<T>();

  {
    Mission_Impossible_Tape<T> mark;

    y = x * y;
    std::cout << tape<T>();

    std::vector<T> diff(20, 0);
    diff[y.index()] = 1;

    tape<T>().reverse(y.index(), diff.data());

    std::cout << "Diff reverse\n" << diff;
  }
}
