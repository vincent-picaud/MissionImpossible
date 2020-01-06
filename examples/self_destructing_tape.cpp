template <typename T>
T
Rosenbrock(const T& x0, const T& x1)
{
  return (1 - x0) * (1 - x0) + 10 * (x1 - x0 * x0) * (x1 - x0 * x0);
}

template <typename T>
T
Rosenbrock_grad(const T& x0, const T& x1)
{
  return (1 - x0) * (1 - x0) + 10 * (x1 - x0 * x0) * (x1 - x0 * x0);
}
