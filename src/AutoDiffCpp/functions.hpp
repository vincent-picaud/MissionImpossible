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

  template <typename T, typename IMPL>
  inline auto
  sin(const AD_Crtp<T, IMPL>& x) noexcept
  {
    using std::sin;

    return AD_Function(sin(x.value()), cos(x.value()) * x.differential());
  }
}
