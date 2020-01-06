#pragma once

#include <cstddef>
#include <type_traits>

namespace Mission_Impossible_AutoDiff
{
  template <typename T, typename IMPL>
  class AD_Crtp;

  template <typename T>
  class AD;

  template <typename T, typename VALUE_TYPE, std::size_t N>
  class AD_Function;

  //================

  template <typename T>
  struct Is_AD : std::false_type
  {
  };
  template <typename T>
  struct Is_AD<AD<T>> : std::true_type
  {
  };

  template <typename T>
  inline constexpr bool Is_AD_v = Is_AD<T>::value;
}
