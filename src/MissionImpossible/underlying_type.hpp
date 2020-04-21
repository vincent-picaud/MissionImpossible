#pragma once

#include "MissionImpossible/ad_fwd.hpp"
#include "MissionImpossible/always.hpp"

#include <type_traits>

namespace MissionImpossible
{
  template <typename T, typename ENABLE = void>
  struct AD_Underlying_Type
  {
    using value_type = T;
  };

  template <typename T>
  struct AD_Underlying_Type<AD<T>, std::enable_if_t<Always_True<typename AD<T>::value_type>::value>>
  {
    using value_type = typename AD_Underlying_Type<typename AD<T>::value_type>::value_type;
  };

  template <typename T, typename VALUE_TYPE, std::size_t N>
  struct AD_Underlying_Type<
      AD_Function<T, VALUE_TYPE, N>,
      std::enable_if_t<Always_True<typename AD_Function<T, VALUE_TYPE, N>::value_type>::value>>
  {
    using value_type =
        typename AD_Underlying_Type<typename AD_Function<T, VALUE_TYPE, N>::value_type>::value_type;
  };

  template <typename T, typename IMPL>
  struct AD_Underlying_Type<
      AD_Crtp<T, IMPL>,
      std::enable_if_t<Always_True<typename AD_Crtp<T, IMPL>::value_type>::value>>
  {
    using value_type =
        typename AD_Underlying_Type<typename AD_Crtp<T, IMPL>::value_type>::value_type;
  };

  template <typename T>
  using AD_Underlying_Type_t = typename AD_Underlying_Type<T>::value_type;

}
