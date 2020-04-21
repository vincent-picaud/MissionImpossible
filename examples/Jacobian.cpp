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

  auto Jacobian_row_y1 = Jacobian_row(y1);  // ∇y1 (or equivalently gradient(y1))
                                            // computes ∂ᵣy¹, ∂ₒy¹

  auto Jacobian_row_y2 = Jacobian_row(y2);  // ∇y2 (or equivalently gradient(y2))
                                            // computes ∂ᵣy², ∂ₒy²

  std::cout << "∇y1(r,θ) = [ ∂ᵣy¹, ∂ₒy¹ ] = " << std::setw(20) << Jacobian_row_y1[r] << ", ";
  std::cout << std::setw(20) << Jacobian_row_y1[theta] << std::endl;

  std::cout << "∇y2(r,θ) = [ ∂ᵣy², ∂ₒy² ] = " << std::setw(20) << Jacobian_row_y2[r] << ", ";
  std::cout << std::setw(20) << Jacobian_row_y2[theta] << std::endl;

  ////////////////////////////////////////
  // Computes Jacobian column by column //
  ////////////////////////////////////////
  //
  // -> AKA forward-mode
  //
  std::cout << std::endl << "Jacobian column by column" << std::endl;

  auto Jacobian_column_r     = Jacobian_column(r);      // r column computes ∂ᵣy¹, ∂ᵣy²
  auto Jacobian_column_theta = Jacobian_column(theta);  // θ column compules ∂ₒy¹, ∂ₒy²

  std::cout << "∂ᵣy¹ = " << std::setw(20) << Jacobian_column_r[y1] << "\t"
            << "∂ᵣy² = " << std::setw(20) << Jacobian_column_theta[y1] << std::endl;
  std::cout << "∂ₒy¹ = " << std::setw(20) << Jacobian_column_r[y2] << "\t"
            << "∂ₒy² = " << std::setw(20) << Jacobian_column_theta[y2] << std::endl;
}
