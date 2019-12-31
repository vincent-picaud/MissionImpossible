#include "AutoDiffCpp/Mission_Impossible.hpp"

#include <benchmark/benchmark.h>

using namespace AutoDiffCpp;

template <typename T>
inline T
Rosenbrock(const T x0, const T x1)
{
  T y = (1 - x0) * (1 - x0) + 10 * (x1 - x0 * x0) * (x1 - x0 * x0);
  return y;
}
template <typename T>
inline auto
Rosenbrock(const T x0, const T x1, T& grad0, T& grad1)
{
  T y   = (1 - x0) * (1 - x0) + 10 * (x1 - x0 * x0) * (x1 - x0 * x0);
  grad0 = -2 * (1 - x0) - 40 * x0 * (-x0 * x0 + x1);
  grad1 = 20 * (-x0 * x0 + x1);
  return y;
}

template <typename T>
inline T
Rosenbrock_ad(const T x0, const T x1, T& grad0, T& grad1)
{
  Mission_Impossible_Tape<T> tape;

  AD<T> _x0 = x0, _x1 = x1, _y;
  _y        = (1 - _x0) * (1 - _x0) + 10 * (_x1 - _x0 * _x0) * (_x1 - _x0 * _x0);
  auto grad = gradient(tape, _y);
  grad0     = grad[_x0];
  grad1     = grad[_x1];
  return _y.value();
}
template <typename T>
inline auto
Rosenbrock(const AD<AD<T>> x0, const AD<AD<T>> x1)
{
  Mission_Impossible_Tape<AD<T>> mark;

  AD<AD<T>> y;
  y = (1 - x0) * (1 - x0) + 10 * (x1 - x0 * x0) * (x1 - x0 * x0);
  return y;
}

static void
BM_Rosenbrock(benchmark::State& state)
{
  double x0 = 3, x1 = 4;
  double grad0, grad1;
  double sum_y = 0, sum_grad0 = 0, sum_grad1 = 0;

  // Perform setup here
  for (auto _ : state)
  {
    for (size_t i = 1; i < 100; i++)
    {
      sum_y += Rosenbrock(x0 / (1. * i), x1 / (1. * i), grad0, grad1);
      sum_grad0 += grad0;
      sum_grad1 += grad1;
    }
  }
  std::cerr << "R:" << sum_y + sum_grad0 + sum_grad1 << std::endl;
}

static void
BM_Rosenbrock_AD(benchmark::State& state)
{
  const double x0 = 3, x1 = 4;
  double grad0, grad1;
  double sum_y = 0, sum_grad0 = 0, sum_grad1 = 0;

  // Perform setup here
  for (auto _ : state)
  {
    for (size_t i = 1; i < 100; i++)
    {
      sum_y += Rosenbrock_ad(x0 / (1. * i), x1 / (1. * i), grad0, grad1);
      sum_grad0 += grad0;
      sum_grad1 += grad1;
    }
  }
  std::cerr << "AD R:" << sum_y + sum_grad0 + sum_grad1 << std::endl;
}

static void
BM_Rosenbrock_AD_2(benchmark::State& state)
{
  const AD<AD<double>> x0 = 3, x1 = 4;

  // Perform setup here
  for (auto _ : state)
  {
    // This code gets timed
    benchmark::DoNotOptimize(Rosenbrock(x0, x1));
  }
}
// Register the function as a benchmark
BENCHMARK(BM_Rosenbrock);
BENCHMARK(BM_Rosenbrock_AD);
BENCHMARK(BM_Rosenbrock_AD_2);
// Run the benchmark
BENCHMARK_MAIN();
