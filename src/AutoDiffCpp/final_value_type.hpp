#pragma once

#include "AutoDiffCpp/ad_fwd.hpp"
#include "AutoDiffCpp/always.hpp"

#include <type_traits>

namespace AutoDiffCpp
{
  template <typename T, typename ENABLE = void>
  struct Final_Value_Type
  {
    using value_type = T;
  };

  template <typename T>
  struct Final_Value_Type<AD<T>, std::enable_if_t<Always_True<typename AD<T>::value_type>::value>>
  {
    using value_type = typename Final_Value_Type<typename AD<T>::value_type>::value_type;
  };

  template <typename T, std::size_t N>
  struct Final_Value_Type<AD_Expr<T, N>,
                          std::enable_if_t<Always_True<typename AD_Expr<T, N>::value_type>::value>>
  {
    using value_type = typename Final_Value_Type<typename AD_Expr<T, N>::value_type>::value_type;
  };

  template <typename T, typename IMPL>
  struct Final_Value_Type<
      AD_Crtp<T, IMPL>,
      std::enable_if_t<Always_True<typename AD_Crtp<T, IMPL>::value_type>::value>>
  {
    using value_type = typename Final_Value_Type<typename AD_Crtp<T, IMPL>::value_type>::value_type;
  };

  template <typename T>
  using Final_Value_Type_t = typename Final_Value_Type<T>::value_type;

}
