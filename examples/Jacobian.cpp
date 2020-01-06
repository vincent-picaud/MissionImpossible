#include "MissionImpossible/Mission_Impossible.hpp"

#include <iostream>

using namespace MissionImpossible;

int
main()
{
  AD<double> r, theta;

  r     = 2;
  theta = 0.1;

  AD<double> y1 = r * cos(theta);
  AD<double> y2 = r * sin(theta);

  //////////////////////////////////
  // Computes Jacobian row by row //
  //////////////////////////////////
  //
  // -> AKA reverse-mode
  //
  std::cout << std::endl << "Jacobian row by row" << std::endl;

  // y1 row (which is also ∇y1)
  // -> you can also replace "Jacobian_row(y1)" by "gradient(y1)"
  //
  auto Jacobian_row_y1 = Jacobian_row(y1);

  std::cout << "∇y1[r] = " << std::setw(20) << Jacobian_row_y1[r]
            << " (must be cos(θ) = " << cos(theta) << ")" << std::endl;
  std::cout << "∇y1[θ] = " << std::setw(20) << Jacobian_row_y1[theta]
            << " (must be -r*sin(θ) = " << -r * sin(theta) << ")" << std::endl;

  // In the same way, one can compute ∇y2
  //
  auto Jacobian_row_y2 = Jacobian_row(y2);

  std::cout << "∇y2[r] = " << std::setw(20) << Jacobian_row_y2[r]
            << " (must be sin(θ) = " << sin(theta) << ")" << std::endl;
  std::cout << "∇y2[θ] = " << std::setw(20) << Jacobian_row_y2[theta]
            << " (must be +r*cos(θ) = " << r * cos(theta) << ")" << std::endl;

  ////////////////////////////////////////
  // Computes Jacobian column by column //
  ////////////////////////////////////////
  //
  // -> AKA forward-mode
  //
  std::cout << std::endl << "Jacobian column by column" << std::endl;

  // r column
  //
  auto Jacobian_column_r = Jacobian_column(r);

  std::cout << "∂r y1  = " << std::setw(20) << Jacobian_column_r[y1]
            << " (must be cos(θ) = " << cos(theta) << ")" << std::endl;
  std::cout << "∂r y2  = " << std::setw(20) << Jacobian_column_r[y2]
            << " (must be sin(θ) = " << sin(theta) << ")" << std::endl;

  // θ column
  //
  auto Jacobian_column_theta = Jacobian_column(theta);

  std::cout << "∂θ y1  = " << std::setw(20) << Jacobian_column_theta[y1]
            << " (must be -r*sin(θ) = " << -r * sin(theta) << ")" << std::endl;
  std::cout << "∂θ y2  = " << std::setw(20) << Jacobian_column_theta[y2]
            << " (must be +r*cos(θ) = " << r * cos(theta) << ")" << std::endl;
}
