#pragma once

namespace MissionImpossible
{
  template <typename T>
  struct Identity
  {
    using type = T;
  };

  template <typename T>
  using Identity_t = typename Identity<T>::type;
}
