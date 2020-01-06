#pragma once

#include "Mission_Impossible_AutoDiff/ad.hpp"

#include <cmath>

namespace Mission_Impossible_AutoDiff
{
  ////////////////////////////
  // Min/max like functions //
  ////////////////////////////

  // TO TEST
  template <typename T, typename IMPL>
  inline AD<T>
  abs(const AD_Crtp<T, IMPL>& x) noexcept
  {
    using std::abs;

    AD<T> type_reduction;
    if (x.value() < 0)
    {
      type_reduction = AD_Function(-x.value(), -x.differential());
    }
    else
    {
      type_reduction = x;
    }
    return type_reduction;
  }

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

  // TODO: max

  //////////////////////////
  // Elementary functions //
  //////////////////////////

  //  TODO float exponent
  template <typename T, typename IMPL>
  inline auto
  pow(const AD_Crtp<T, IMPL>& x, const int iexp) noexcept
  {
    using std::pow;

    return AD_Function(pow(x.value(), iexp), iexp * pow(x.value(), iexp - 1) * x.differential());
  }

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
  log(const AD_Crtp<T, IMPL>& x) noexcept
  {
    using std::log;

    return AD_Function(log(x.value()), (1 / x.value()) * x.differential());
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

  template <typename T, typename IMPL>
  inline auto
  tan(const AD_Crtp<T, IMPL>& x) noexcept
  {
    using std::cos;
    using std::tan;
    using std::pow;

    return AD_Function(tan(x.value()), 1 / pow(cos(x.value()), 2) * x.differential());
  }

}
