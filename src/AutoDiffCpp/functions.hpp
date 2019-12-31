#pragma once

#include "AutoDiffCpp/ad.hpp"

#include <cmath>

namespace AutoDiffCpp
{
  template <typename T, typename IMPL>
  inline auto
  exp(const AD_Crtp<T, IMPL>& x) noexcept
  {
    using std::exp;

    const auto v = exp(x.value());
    return AD_Function(v, v * x.differential());
  }
}
