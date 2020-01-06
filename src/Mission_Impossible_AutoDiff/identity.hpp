#pragma once

namespace Mission_Impossible_AutoDiff
{
  template <typename T>
  struct Identity
  {
    using type = T;
  };

  template <typename T>
  using Identity_t = typename Identity<T>::type;
}
