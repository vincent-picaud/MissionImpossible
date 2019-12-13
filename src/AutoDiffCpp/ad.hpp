#pragma once

#include "AutoDiffCpp/identity.hpp"
#include "AutoDiffCpp/tape.hpp"
#include "AutoDiffCpp/type_traits.hpp"

#include <array>

namespace AutoDiffCpp
{
  /////////////
  // AD_Crtp //
  /////////////
  //
  template <typename T, typename IMPL>
  class AD_Crtp
  {
   public:
    using type_traits = Type_Traits<IMPL>;
    // using value_type = typename type_traits::value_type;
    // using offset_type = typename type_traits::value_type;
    // using value_type = typename type_traits::value_type;
    // using value_type = typename type_traits::value_type;
    //  using arr
   public:
    IMPL&
    impl()
    {
      return static_cast<IMPL&>(*this);
    };
    const IMPL&
    impl() const
    {
      return static_cast<IMPL&>(*this);
    };
  };

  ////////
  // AD //
  ////////

  template <typename T>
  class AD;
  template <typename T, size_t N>
  class AD_Expr;

  template <typename T>
  class AD : public AD_Crtp<T, AD<T>>
  {
   protected:
    static thread_local Tape<T> _tape;
    T _value;
    offset_type _index;

   public:
    //    AD(const AD&) = default;
    AD(const T& value)
    {
      _value = value;
      _index = _tape.add_variable();
    }

    T
    value() const
    {
      return _value;
    };
    offset_type
    index() const
    {
      return _index;
    };
  };

  template <typename T>
  thread_local Tape<T> AD<T>::_tape = Tape<T>();

  /////////////
  // AD_Expr //
  /////////////

  template <typename T, size_t N>
  class AD_Expr : public AD_Crtp<T, AD_Expr<T, N>>
  {
   protected:
    T _value;
    std::array<offset_type, N> _offset;
    std::array<T, N> _partial;

   public:
    //    AD_Expr(const AD_Expr&) = default;

    AD_Expr(const T value,
            const std::array<offset_type, N>& offset,
            const std::array<T, N>& partial)
        : _value(value), _offset(offset), _partial(partial)
    {
    }

    T
    value() const
    {
      return _value;
    };
  };

  template <typename T, size_t N>
  inline std::array<T, N> operator*(const Identity_t<T> alpha, const std::array<T, N>& vector)
  {
    std::array<T, N> dest;
    for (std::size_t i = 0; i < N; ++i)
    {
      dest[i] = alpha * vector[i];
    }
    return dest;
  }

  // f:R2->R
  //
  // df○g = ∂1f.dg^1 + ∂2f.dg^2
  //
  template <typename T>
  inline auto
  chain_rule(const typename AD<T>::value_type f_circ_g_value,
             const typename AD<T>::value_type partial1,
             const AD<T>& g1,
             const typename AD<T>::value_type partial2,
             const AD<T>& g2)
  {
  }

  template <typename T>
  AD_Expr<T, 1> operator*(const Identity_t<T> a0, const AD<T>& a1)
  {
    return AD_Expr<T, 1>{
        a0 * a1.value(), std::array<offset_type, 1>({a1.index()}), std::array<T, 1>({a0})};
  }

  template <typename T, typename IMPL0, typename IMPL1>
  AD_Expr<T, 1> operator*(const AD_Crtp<T, IMPL0>& a0, const AD_Crtp<T, IMPL1>& a1)
  {
    return AD_Expr<T, 1>{
        a0 * a1.value(), std::array<offset_type, 1>({a1.index()}), std::array<T, 1>({a0})};
  }
}  // namespace AutoDiffCpp
