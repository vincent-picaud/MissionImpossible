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
    static_cast<std::array<T, N>&>(*this) = v;
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
TinyVector<T, N> operator*(const T scalar, const TinyVector<T, N>& A)
{
  TinyVector<T, N> aA;
  for (size_t i = 0; i < N; i++)
  {
    aA[i] = scalar * A[i];
  }
  return aA;
}

template <typename T, std::size_t N, std::size_t M>
TinyMatrix<T, N, M> operator*(const T scalar, const TinyMatrix<T, N, M>& A)
{
  TinyMatrix<T, N, M> aA;
  for (size_t i = 0; i < N; i++)
  {
    aA[i] = scalar * A[i];
  }
  return aA;
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
template <typename T, std::size_t N>
TinyVector<T, N>
operator-(const TinyVector<T, N>& u, const TinyVector<T, N>& v)
{
  TinyVector<T, N> w;

  for (size_t i = 0; i < N; i++)
  {
    w[i] = u[i] - v[i];
  }
  return w;
}

template <typename T, std::size_t N, std::size_t M>
TinyMatrix<T, N, M>
operator-(const TinyMatrix<T, N, M>& u, const TinyMatrix<T, N, M>& v)
{
  TinyMatrix<T, N, M> w;

  for (size_t i = 0; i < N; i++)
  {
    w[i] = u[i] - v[i];
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

//================================================================

template <typename T, std::size_t N>
TinyVector<AD<T>, N>
to_AD(const TinyVector<T, N>& v)
{
  TinyVector<AD<T>, N> ad_v;
  for (size_t i = 0; i < N; i++)
  {
    ad_v[i] = v[i];
  }
  return ad_v;
}

template <typename T, std::size_t N, std::size_t M>
TinyMatrix<AD<T>, N, M>
to_AD(const TinyMatrix<T, N, M>& v)
{
  TinyMatrix<AD<T>, N, M> ad_v;
  for (size_t i = 0; i < N; i++)
  {
    ad_v[i] = to_AD(v[i]);
  }
  return ad_v;
}

template <typename T, std::size_t N>
TinyVector<T, N>
extract_grad(const TinyVector<AD<T>, N>& v, const Tape_Vector<T>& grad)
{
  TinyVector<T, N> extracted_grad;
  for (size_t i = 0; i < N; i++)
  {
    extracted_grad[i] = grad[v[i]];
  }
  return extracted_grad;
}
template <typename T, std::size_t N, std::size_t M>
TinyMatrix<T, N, M>
extract_grad(const TinyMatrix<AD<T>, N, M>& v, const Tape_Vector<T>& grad)
{
  TinyMatrix<T, N, M> extracted_grad;
  for (size_t i = 0; i < N; i++)
  {
    extracted_grad[i] = extract_grad(v[i], grad);
  }
  return extracted_grad;
}
//================================================================
template <typename T, std::size_t N, std::size_t M>
TinyVector<T, N>
layer(const TinyMatrix<T, N, M>& W, const TinyVector<T, N>& b, const TinyVector<T, M>& x)
{
  return sigmoid(W * x + b);
}

template <typename T>
TinyVector<T, 2>
network(const TinyMatrix<T, 3, 2>& W2,
        const TinyVector<T, 3>& b2,
        const TinyMatrix<T, 2, 3>& W3,
        const TinyVector<T, 2>& b3,
        const TinyMatrix<T, 2, 2>& W4,
        const TinyVector<T, 2>& b4,
        const TinyVector<T, 2>& x)
{
  return layer(W4, b4, layer(W3, b3, layer(W2, b2, x)));
}
// template <typename T>
// T
// network(const T& x)

template <typename T>
T
network(const TinyMatrix<T, 3, 2>& W2,
        const TinyVector<T, 3>& b2,
        const TinyMatrix<T, 2, 3>& W3,
        const TinyVector<T, 2>& b3,
        const TinyMatrix<T, 2, 2>& W4,
        const TinyVector<T, 2>& b4,
        const TinyVector<T, 2>& x,
        const TinyVector<T, 2>& y)
{
  const auto pred = network(W2, b2, W3, b3, W4, b4, x);
  const auto cost = pow(pred[0] - y[0], 2) + pow(pred[1] - y[1], 2);
  return cost;
}

// CAVEAT: for gradient use ONE sample, but plot GLOBAL cost
template <typename T, std::size_t N>
T
global_cost(const TinyMatrix<T, 3, 2>& W2,
            const TinyVector<T, 3>& b2,
            const TinyMatrix<T, 2, 3>& W3,
            const TinyVector<T, 2>& b3,
            const TinyMatrix<T, 2, 2>& W4,
            const TinyVector<T, 2>& b4,
            const TinyMatrix<T, N, 2>& X,
            const TinyMatrix<T, N, 2>& y)
{
  T sum = 0;
  for (size_t i = 0; i < N; ++i)
  {
    const auto pred = network(W2, b2, W3, b3, W4, b4, X[i]);
    sum += pow(pred[0] - y[i][0], 2) + pow(pred[1] - y[i][1], 2);
  }
  return sum;
}

template <typename T, std::size_t N>
T
iterate(const T& eta,
        TinyMatrix<T, 3, 2>& W2,
        TinyVector<T, 3>& b2,
        TinyMatrix<T, 2, 3>& W3,
        TinyVector<T, 2>& b3,
        TinyMatrix<T, 2, 2>& W4,
        TinyVector<T, 2>& b4,
        const TinyMatrix<T, N, 2>& X,
        const TinyMatrix<T, N, 2>& y,
        const std::size_t sample_idx)
{
  Mission_Impossible_Tape<T> local_tape;

  auto ad_W2 = to_AD(W2);
  auto ad_W3 = to_AD(W3);
  auto ad_W4 = to_AD(W4);
  auto ad_b2 = to_AD(b2);
  auto ad_b3 = to_AD(b3);
  auto ad_b4 = to_AD(b4);
  auto ad_x  = to_AD(X[sample_idx]);
  auto ad_y  = to_AD(y[sample_idx]);

  auto cost = network(ad_W2, ad_b2, ad_W3, ad_b3, ad_W4, ad_b4, ad_x, ad_y);

  //  std::cerr << "cost: " << cost;

  auto grad_cost = gradient(cost);

  W2 = W2 - eta * extract_grad(ad_W2, grad_cost);
  W3 = W3 - eta * extract_grad(ad_W3, grad_cost);
  W4 = W4 - eta * extract_grad(ad_W4, grad_cost);
  b2 = b2 - eta * extract_grad(ad_b2, grad_cost);
  b3 = b3 - eta * extract_grad(ad_b3, grad_cost);
  b4 = b4 - eta * extract_grad(ad_b4, grad_cost);

  return global_cost(W2, b2, W3, b3, W4, b4, X, y);
}

//////////////////////////////////////////////////////////////////

auto
data_XY()
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

  TinyMatrix<double, 10, 2> Y;
  Y[0] = std::array{1., 0.};
  Y[1] = std::array{1., 0.};
  Y[2] = std::array{1., 0.};
  Y[3] = std::array{1., 0.};
  Y[4] = std::array{1., 0.};
  Y[5] = std::array{0., 1.};
  Y[6] = std::array{0., 1.};
  Y[7] = std::array{0., 1.};
  Y[8] = std::array{0., 1.};
  Y[9] = std::array{0., 1.};

  return std::pair(X, Y);
}

int
main()
{
  using T = double;
  //  using T = AD<double>;

  TinyMatrix<T, 3, 2> W2;
  TinyVector<T, 3> b2;
  TinyMatrix<T, 2, 3> W3;
  TinyVector<T, 2> b3;
  TinyMatrix<T, 2, 2> W4;
  TinyVector<T, 2> b4;

  initialize(W2);
  initialize(W3);
  initialize(W4);
  initialize(b2);
  initialize(b3);
  initialize(b4);

  auto XY = data_XY();

  const size_t n_data = XY.first.size();
  std::uniform_int_distribution<size_t> random_sample(0, n_data - 1);

  const T eta = 1.5;

  for (std::size_t iter = 0; iter < 1e5; ++iter)
  {
    const size_t sample_idx = random_sample(gen);
    [[maybe_unused]] const auto cost =
        iterate(eta, W2, b2, W3, b3, W4, b4, XY.first, XY.second, sample_idx);

    // Gnuplot:
    // gnuplot> set logscale y 10
    // gnuplot> plot "cv.txt" u 1:3 w l
    
    std::cerr << "\n" << iter << " " << sample_idx << " " << cost;
  }

  return 0;

  // Classifier output
  //
  std::cout << std::endl;
  const double grid_N = 60;
  const double grid_M = 60;
  for (std::size_t j = 0; j <= grid_M; ++j)
  {
    for (std::size_t i = 0; i <= grid_N; ++i)
    {
      TinyVector<T, 2> x{i / grid_N, j / grid_M};
      const auto y = network(W2, b2, W3, b3, W4, b4, x);
      std::cout << (y[0] > y[1] ? 1 : 0) << " ";
    }
    std::cout << std::endl;
  }
}
