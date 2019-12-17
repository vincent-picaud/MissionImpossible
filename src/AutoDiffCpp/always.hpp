#pragma once

#include <type_traits>

namespace AutoDiffCpp
{
  template <typename T>
  struct Always_True : std::true_type
  {
  };

  template <typename T>
  inline constexpr bool Always_True_v = Always_True<T>::value;

  template <typename T>
  struct Always_False : std::false_type
  {
  };

  template <typename T>
  inline constexpr bool Always_False_v = Always_False<T>::value;
}
