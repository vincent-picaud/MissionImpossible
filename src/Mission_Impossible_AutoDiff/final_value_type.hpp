#pragma once

#include "Mission_Impossible_AutoDiff/ad_fwd.hpp"
#include "Mission_Impossible_AutoDiff/always.hpp"

#include <type_traits>

namespace Mission_Impossible_AutoDiff
{
  template <typename T, typename ENABLE = void>
  struct AD_Final_Value_Type
  {
    using value_type = T;
  };

  template <typename T>
  struct AD_Final_Value_Type<AD<T>,
                             std::enable_if_t<Always_True<typename AD<T>::value_type>::value>>
  {
    using value_type = typename AD_Final_Value_Type<typename AD<T>::value_type>::value_type;
  };

  template <typename T, typename VALUE_TYPE, std::size_t N>
  struct AD_Final_Value_Type<
      AD_Function<T, VALUE_TYPE, N>,
      std::enable_if_t<Always_True<typename AD_Function<T, VALUE_TYPE, N>::value_type>::value>>
  {
    using value_type = typename AD_Final_Value_Type<
        typename AD_Function<T, VALUE_TYPE, N>::value_type>::value_type;
  };

  template <typename T, typename IMPL>
  struct AD_Final_Value_Type<
      AD_Crtp<T, IMPL>,
      std::enable_if_t<Always_True<typename AD_Crtp<T, IMPL>::value_type>::value>>
  {
    using value_type =
        typename AD_Final_Value_Type<typename AD_Crtp<T, IMPL>::value_type>::value_type;
  };

  template <typename T>
  using AD_Final_Value_Type_t = typename AD_Final_Value_Type<T>::value_type;

}
