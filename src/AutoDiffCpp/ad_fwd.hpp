#pragma once

#include <cstddef>

namespace AutoDiffCpp
{
  template <typename T, typename IMPL>
  class AD_Crtp;

  template <typename T>
  class AD;

  template <typename T, std::size_t N>
  class AD_Expr;
}
