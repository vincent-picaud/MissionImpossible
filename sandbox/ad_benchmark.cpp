#include <benchmark/benchmark.h>

template <typename T>
inline auto
Rosenbrock(const T x0, const T x1)
{
  return (1 - x0) * (1 - x0) + 10 * (x1 - x0 * x0) * (x1 - x0 * x0);
}

static void
BM_SomeFunction(benchmark::State& state)
{
  const double x0 = 3, x1 = 4;

  // Perform setup here
  for (auto _ : state)
  {
    // This code gets timed
    Rosenbrock(x0, x1);
  }
}
// Register the function as a benchmark
BENCHMARK(BM_SomeFunction);
// Run the benchmark
BENCHMARK_MAIN();
