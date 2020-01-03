// A tiny "deep learning" example
//
// Reproduce the example given in
// #+begin_quote
// "Deep Learning: An Introduction for Applied Mathematicians",
//  https://arxiv.org/abs/1801.05894
// #+end_quote
//
#include "AutoDiffCpp/Mission_Impossible.hpp"

#include <array>
#include <random>

using namespace AutoDiffCpp;

//////////////////////////////////////////////////////////////////

std::random_device rd{};
std::mt19937 gen{rd()};
std::normal_distribution<> d{0, 1};

//////////////////////////////////////////////////////////////////

template <typename T, std::size_t N>
struct TinyVector : public std::array<T, N>
{
  friend std::ostream&
  operator<<(std::ostream& out, const TinyVector& tv)
  {
    for (const auto& tv_i : tv)
    {
      out << std::setw(20) << tv_i << " ";
    }
    return out;
  }
};

template <typename T, std::size_t N, std::size_t M>
struct TinyMatrix : std::array<TinyVector<T, N>, M>
{
  friend std::ostream&
  operator<<(std::ostream& out, const TinyMatrix& tv)
  {
    for (const auto& tv_i : tv)
    {
      out << tv_i << std::endl;
    }
    return out;
  }
};

template <typename T, std::size_t N>
void
initialize(TinyVector<T, N>& a)
{
  for (auto& a_i : a)
  {
    a_i = 0.5 * d(gen);
  }
}
template <typename T, std::size_t N, std::size_t M>
void
initialize(TinyMatrix<T, N, M>& a)
{
  for (auto& a_i : a)
  {
    initialize(a_i);
  }
}

int
main()
{
  using T = double;

  TinyMatrix<AD<T>, 2, 3> V;

  initialize(V);

  std::cout << V;
}
