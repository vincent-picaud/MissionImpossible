#include "MissionImpossible/MissionImpossible.hpp"

#include <iostream>

using namespace MissionImpossible;

int
main()
{
  AD<double> r = 2, theta = 0.1;

  AD<double> y1 = r * cos(theta);
  AD<double> y2 = r * sin(theta);

  //////////////////////////////////
  // Computes Jacobian row by row //
  //////////////////////////////////
  //
  // -> AKA reverse-mode
  //
  std::cout << "Jacobian row by row" << std::endl;

  auto Jacobian_row_y1 = Jacobian_row(y1);  // ∇y1

  std::cout << "∇y1(r,θ) = " << std::setw(20) << Jacobian_row_y1[r] << ", ";
  std::cout << std::setw(20) << Jacobian_row_y1[theta] << std::endl;

  auto Jacobian_row_y2 = Jacobian_row(y2);  // ∇y2

  std::cout << "∇y2(r,θ) = " << std::setw(20) << Jacobian_row_y2[r] << ", ";
  std::cout << std::setw(20) << Jacobian_row_y2[theta] << std::endl;

  ////////////////////////////////////////
  // Computes Jacobian column by column //
  ////////////////////////////////////////
  //
  // -> AKA forward-mode
  //
  std::cout << std::endl << "Jacobian column by column" << std::endl;

  auto Jacobian_column_r = Jacobian_column(r);  // r column

  std::cout << "∂r y1  = " << std::setw(20) << Jacobian_column_r[y1] << std::endl;
  std::cout << "∂r y2  = " << std::setw(20) << Jacobian_column_r[y2] << std::endl;

  auto Jacobian_column_theta = Jacobian_column(theta);  // θ column

  std::cout << "∂θ y1  = " << std::setw(20) << Jacobian_column_theta[y1] << std::endl;
  std::cout << "∂θ y2  = " << std::setw(20) << Jacobian_column_theta[y2] << std::endl;
}
