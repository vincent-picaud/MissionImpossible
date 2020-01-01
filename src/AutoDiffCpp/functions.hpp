#pragma once

#include "AutoDiffCpp/ad.hpp"

#include <cmath>

namespace AutoDiffCpp
{
  ////////////////////////////
  // Min/max like functions //
  ////////////////////////////

  // Explain why it also works for NaN
  //
  template <typename T, typename IMPL>
  inline auto
  min(const AD_Final_Value_Type_t<T> x0, const AD_Crtp<T, IMPL>& x1) noexcept
  {
    using std::min;

    AD<T> type_reduction;
    if (x0.value() == min(x0.value(), x1.value()))
      type_reduction = x0;
    else
      type_reduction = AD_Function(x1.value(), x1.differential());
    return type_reduction;
  }
  template <typename T, typename IMPL>
  inline auto
  min(const AD_Crtp<T, IMPL>& x1, const AD_Final_Value_Type_t<T> x0) noexcept
  {
    using std::min;

    AD<T> type_reduction;
    if (x0.value() == min(x0.value(), x1.value()))
      type_reduction = AD_Function(x0.value(), x0.differential());
    else
      type_reduction = x1;
    return type_reduction;
  }

  template <typename T, typename IMPL0, typename IMPL1>
  inline auto
  min(const AD_Crtp<T, IMPL0>& x0, const AD_Crtp<T, IMPL1>& x1) noexcept
  {
    using std::min;

    AD<T> type_reduction;
    if (x0.value() == min(x0.value(), x1.value()))
      type_reduction = AD_Function(x0.value(), x0.differential());
    else
      type_reduction = AD_Function(x1.value(), x1.differential());
    return type_reduction;
  }

  //////////////////////////
  // Elementary functions //
  //////////////////////////

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
    using std::cos;
    using std::sin;

    return AD_Function(sin(x.value()), cos(x.value()) * x.differential());
  }

  template <typename T, typename IMPL>
  inline auto
  cos(const AD_Crtp<T, IMPL>& x) noexcept
  {
    using std::cos;
    using std::sin;

    return AD_Function(cos(x.value()), -sin(x.value()) * x.differential());
  }
}
