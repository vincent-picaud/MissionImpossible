#include "AutoDiffCpp/Mission_Impossible.hpp"

#include <benchmark/benchmark.h>

using namespace AutoDiffCpp;

template <typename T>
inline auto
Rosenbrock(const T x0, const T x1)
{
  T y = (1 - x0) * (1 - x0) + 10 * (x1 - x0 * x0) * (x1 - x0 * x0);
  return y;
}

template <typename T>
inline auto
Rosenbrock(const AD<T> x0, const AD<T> x1)
{
  Mission_Impossible_Tape<T> mark;

  AD<T> y;
  y = (1 - x0) * (1 - x0) + 10 * (x1 - x0 * x0) * (x1 - x0 * x0);
  return y;
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
  const double x0 = 3, x1 = 4;

  // Perform setup here
  for (auto _ : state)
  {
    // This code gets timed
    benchmark::DoNotOptimize(Rosenbrock(x0, x1));
  }
}

static void
BM_Rosenbrock_AD(benchmark::State& state)
{
  const AD<double> x0 = 3, x1 = 4;

  // Perform setup here
  for (auto _ : state)
  {
    // This code gets timed
    benchmark::DoNotOptimize(Rosenbrock(x0, x1));
  }
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
