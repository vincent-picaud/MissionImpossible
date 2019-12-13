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
   public:
    using tape_type   = Tape<T>;
    using offset_type = typename tape_type::offset_type;
    using value_type  = typename tape_type::value_type;

   private:
    static thread_local tape_type _tape;
    value_type _value;
    offset_type _index;

   public:
    //    AD(const AD&) = default;
    AD(const value_type& value)
    {
      _value = value;
      _index = _tape.add_variable();
    }

    value_type
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
  thread_local typename AD<T>::tape_type AD<T>::_tape = AD<T>::tape_type();

  /////////////
  // AD_Expr //
  /////////////

  template <typename T, size_t N>
  class AD_Expr : public AD_Crtp<T, AD_Expr<T, N>>
  {
   public:
    using offset_type         = typename AD<T>::offset_type;
    using value_type          = typename AD<T>::value_type;
    using offset_array_type   = std::array<offset_type, N>;
    using partialD_array_type = std::array<value_type, N>;

   protected:
    T _value;
    offset_array_type _offset_array;
    partialD_array_type _partialD_array;

   public:
    //    AD_Expr(const AD_Expr&) = default;

    AD_Expr(const T value,
            const offset_array_type& offset_array,
            const partialD_array_type& partialD_array)
        : _value(value), _offset_array(offset_array), _partialD_array(partialD_array)
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
  inline  AD_Expr<T, 1>
  chain_rule(const Identity_t<T> f_circ_g_value,
             const Identity_t<T> partial1,
             const AD<T>& g1,
             const Identity_t<T> partial2,
             const AD<T>& g2)
  {
  }

  template <typename T>
  AD_Expr<T, 1> operator*(const Identity_t<T> a0, const AD<T>& a1)
  {
    using return_type         = decltype(a0 * a1);
    using offset_array_type   = typename return_type::offset_array_type;
    using partialD_array_type = typename return_type::partialD_array_type;

    return return_type{a0 * a1.value(), offset_array_type({a1.index()}), partialD_array_type({a0})};
  }

  // template <typename T, typename IMPL0, typename IMPL1>
  // AD_Expr<T, 1> operator*(const AD_Crtp<T, IMPL0>& a0, const AD_Crtp<T, IMPL1>& a1)
  // {
  //   return AD_Expr<T, 1>{
  //       a0 * a1.value(), std::array<offset_type, 1>({a1.index()}), std::array<T, 1>({a0})};
  // }
}  // namespace AutoDiffCpp
