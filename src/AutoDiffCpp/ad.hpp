#pragma once

#include "AutoDiffCpp/tape.hpp"

#include <array>

namespace AutoDiffCpp
{
  /////////////
  // AD_Crtp //
  /////////////
  template <typename T, typename IMPL>
  class AD_Crtp
  {
   public:
    using value_type = T;

   public:
    IMPL& impl() { return static_cast<IMPL&>(*this); };
    const IMPL& impl() const { return static_cast<IMPL&>(*this); };
  };

  ////////
  // AD //
  ////////

  // template <typename T>
  // struct Identity
  // {
  //   using type = T;
  // };
  // template <typename T>
  // Identity_t = typename Identity<T>::type;

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

    T value() const { return _value; };
    offset_type index() const { return _index; };
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

    T value() const { return _value; };
  };

  template <typename T>
  AD_Expr<T, 1> operator*(const typename AD<T>::value_type a0, const AD<T>& a1)
  {
    return AD_Expr<T, 1>{a0 * a1.value(),
                         std::array<offset_type, 1>({a1.index()}),
                         std::array<T, 1>({a0})};
  }
}  // namespace AutoDiffCpp
