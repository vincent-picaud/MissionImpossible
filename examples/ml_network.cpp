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
  TinyVector
  operator=(const std::array<T, N>& v)
  {
    (*this) = v;
    return *this;
  }

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
struct TinyMatrix : std::array<TinyVector<T, M>, N>
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

template <typename T, std::size_t N>
T
dot(const TinyVector<T, N>& v, const TinyVector<T, N>& w)
{
  T sum = 0;

  for (size_t i = 0; i < N; i++)
  {
    sum += v[i] * w[i];
  }
  return sum;
}

template <typename T, std::size_t N, std::size_t M>
TinyVector<T, N> operator*(const TinyMatrix<T, N, M>& A, const TinyVector<T, M>& v)
{
  TinyVector<T, N> w;
  for (size_t i = 0; i < N; i++)
  {
    w[i] = dot(A[i], v);
  }
  return w;
}
template <typename T, std::size_t N>
TinyVector<T, N>
operator+(const TinyVector<T, N>& u, const TinyVector<T, N>& v)
{
  TinyVector<T, N> w;

  for (size_t i = 0; i < N; i++)
  {
    w[i] = u[i] + v[i];
  }
  return w;
}

template <typename T>
T
sigmoid(const T& x)
{
  return 1 / (1 + exp(-x));
}

template <typename T, std::size_t N>
TinyVector<T, N>
sigmoid(const TinyVector<T, N>& v)
{
  TinyVector<T, N> w;
  for (size_t i = 0; i < N; i++)
  {
    w[i] = sigmoid(v[i]);
  }
  return w;
}

template <typename T, std::size_t N, std::size_t M>
TinyVector<T, N>
layer(const TinyMatrix<T, N, M>& W, const TinyVector<T, N>& b, const TinyVector<T, M>& x)
{
  return sigmoid(W * x + b);
}

// template <typename T>
// T
// network(const T& x)

template <typename T, std::size_t N, std::size_t M, typename CLASSIFIER>
T
cost_quadratic(const TinyMatrix<T, N, M>& X,
               const TinyVector<T, N>& y,
               const CLASSIFIER& classifier)
{
  T sum = 0;
  for (size_t i = 0; i < N; i++)
  {
    sum += pow(y[i] - classifier(X[i]), 2);
  }

  return sum / N;
}

//////////////////////////////////////////////////////////////////

auto
data_X()
{
  TinyMatrix<double, 10, 2> X;
  X[0] = std::array{0.1, 0.1};
  X[1] = std::array{0.3, 0.4};
  X[2] = std::array{0.1, 0.5};
  X[3] = std::array{0.6, 0.9};
  X[4] = std::array{0.4, 0.2};
  X[5] = std::array{0.6, 0.3};
  X[6] = std::array{0.5, 0.6};
  X[7] = std::array{0.9, 0.2};
  X[8] = std::array{0.4, 0.4};
  X[9] = std::array{0.7, 0.6};

  // std::array{1, 2};
  return X;
}
int
main()
{
  //  using T = double;
  using T = AD<double>;

  TinyVector<T, 3> x;
  TinyVector<T, 2> b;
  TinyMatrix<T, 2, 3> M;

  initialize(x);
  initialize(b);
  initialize(M);

  auto X = data_X();
  std::cout << layer(M, b, x);
  std::cout << X;
}
