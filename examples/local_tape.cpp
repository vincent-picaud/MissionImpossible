#include "MissionImpossible/MissionImpossible.hpp"

#include <iostream>
#include <array>

using namespace MissionImpossible;

// template<typename T>
// T Rosenbrock(const std::array<T>& X,
void
example(const double x,
        const double mu,
        const double sigma,
        double& f,
        double& grad_f_mu,
        double& grad_f_sigma)
{
  MissionImpossible_Tape<double> local_tape;  // a local thread_local tape

  AD<double> ad_mu(mu), ad_sigma(sigma), ad_f;

  ad_f = 2 * exp(-(x - ad_mu) / pow(ad_sigma, 2));

  auto ad_grad_f = gradient(local_tape, ad_f); // use the local tape for ∇f

  f            = ad_f.value();
  grad_f_mu    = ad_grad_f[ad_mu];
  grad_f_sigma = ad_grad_f[ad_sigma];

  // here the local tape is destroyed (in fact re-winded to avoid
  // useless new/delete)
}

int
main()
{
  double x = 1, mu = 2, sigma = 3;
  double f, grad_f_mu, grad_f_sigma;

  example(x, mu, sigma, f, grad_f_mu, grad_f_sigma);

  std::cout << "f  = " << f << std::endl;
  std::cout << "∇f = " << grad_f_mu << ", " << grad_f_sigma;
}
