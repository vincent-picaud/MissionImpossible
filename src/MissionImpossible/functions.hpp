//
// TODO: to complete... + special functions
//
#pragma once

#include "MissionImpossible/ad.hpp"

#include <cmath>

namespace MissionImpossible
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
      type_reduction = x.impl();
    }
    return type_reduction;
  }

  // TODO: explain why min/max also work with NaN args
  //

  // min
  template <typename T, typename IMPL>
  inline AD<T>
  min(const AD_Final_Value_Type_t<T> x0, const AD_Crtp<T, IMPL>& x1) noexcept
  {
    using std::min;

    if (x0 == min(x0, x1.value())) return {x0};
    return {x1.impl()};
  }
  template <typename T, typename IMPL>
  inline AD<T>
  min(const AD_Crtp<T, IMPL>& x1, const AD_Final_Value_Type_t<T> x0) noexcept
  {
    return min(x1, x0);
  }

  template <typename T, typename IMPL0, typename IMPL1>
  inline AD<T>
  min(const AD_Crtp<T, IMPL0>& x0, const AD_Crtp<T, IMPL1>& x1) noexcept
  {
    using std::min;

    if (x0.value() == min(x0.value(), x1.value())) return {x0.impl()};
    return {x1.impl()};
  }

  // max
  template <typename T, typename IMPL>
  inline AD<T>
  max(const AD_Final_Value_Type_t<T> x0, const AD_Crtp<T, IMPL>& x1) noexcept
  {
    using std::max;

    if (x0 == max(x0, x1.value())) return {x0};
    return {x1.impl()};
  }
  template <typename T, typename IMPL>
  inline AD<T>
  max(const AD_Crtp<T, IMPL>& x1, const AD_Final_Value_Type_t<T> x0) noexcept
  {
    return max(x1, x0);
  }

  template <typename T, typename IMPL0, typename IMPL1>
  inline AD<T>
  max(const AD_Crtp<T, IMPL0>& x0, const AD_Crtp<T, IMPL1>& x1) noexcept
  {
    using std::max;

    if (x0.value() == max(x0.value(), x1.value())) return {x0.impl()};
    return {x1.impl()};
  }
  
  //////////////////////////
  // Elementary functions //
  //////////////////////////

  template <typename T, typename IMPL>
  inline auto
  sqrt(const AD_Crtp<T, IMPL>& x) noexcept
  {
    using std::sqrt;

    return AD_Function(sqrt(x.value()), 1 / (2 * sqrt(x.value())) * x.differential());
  }

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
    using std::pow;
    using std::tan;

    return AD_Function(tan(x.value()), 1 / pow(cos(x.value()), 2) * x.differential());
  }

  // TODO: After this mark, not tested
  template <typename T, typename IMPL>
  inline auto
  acos(const AD_Crtp<T, IMPL>& x) noexcept
  {
    using std::pow;
    using std::sqrt;

    return AD_Function(acos(x.value()), -1 / sqrt(1 - pow(x.value(), 2)) * x.differential());
  }

  template <typename T, typename IMPL>
  inline auto
  asin(const AD_Crtp<T, IMPL>& x) noexcept
  {
    using std::pow;
    using std::sqrt;

    return AD_Function(asin(x.value()), 1 / sqrt(1 - pow(x.value(), 2)) * x.differential());
  }

  template <typename T, typename IMPL>
  inline auto
  atan(const AD_Crtp<T, IMPL>& x) noexcept
  {
    using std::pow;

    return AD_Function(atan(x.value()), 1 / (1 + pow(x.value(), 2)) * x.differential());
  }

  template <typename T, typename IMPL>
  inline auto
  cosh(const AD_Crtp<T, IMPL>& x) noexcept
  {
    using std::cosh;
    using std::sinh;

    return AD_Function(cosh(x.value()), sinh(x.value()) * x.differential());
  }

  template <typename T, typename IMPL>
  inline auto
  sinh(const AD_Crtp<T, IMPL>& x) noexcept
  {
    using std::cosh;
    using std::sinh;

    return AD_Function(sinh(x.value()), cosh(x.value()) * x.differential());
  }

  template <typename T, typename IMPL>
  inline auto
  tanh(const AD_Crtp<T, IMPL>& x) noexcept
  {
    using std::cosh;
    using std::pow;
    using std::tanh;

    return AD_Function(tanh(x.value()), 1 / pow(cosh(x.value()), 2) * x.differential());
  }
  template <typename T, typename IMPL>
  inline auto
  atanh(const AD_Crtp<T, IMPL>& x) noexcept
  {
    using std::atanh;
    using std::pow;

    return AD_Function(atanh(x.value()), 1 / (1 - pow(x.value(), 2)) * x.differential());
  }
  template <typename T, typename IMPL>
  inline auto
  acosh(const AD_Crtp<T, IMPL>& x) noexcept
  {
    using std::atanh;
    using std::sqrt;

    return AD_Function(acosh(x.value()),
                       1 / sqrt((x.value() - 1) * (x.value() + 1)) * x.differential());
  }
  template <typename T, typename IMPL>
  inline auto
  asinh(const AD_Crtp<T, IMPL>& x) noexcept
  {
    using std::atanh;
    using std::pow;
    using std::sqrt;

    return AD_Function(asinh(x.value()), 1 / sqrt(pow(x.value() + 1, 2)) * x.differential());
  }
}
