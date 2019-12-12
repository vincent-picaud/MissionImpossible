#pragma once

#include "AutoDiffCpp/tape.hpp"

namespace AutoDiffCpp
{
  template <typename T, typename IMPL>
  class AD_Crtp
  {
   public:
    IMPL& impl() { return static_cast<IMPL&>(*this); };
    const IMPL& impl() const { return static_cast<IMPL&>(*this); };
  };

  template <typename T>
  class AD : public AD_Crtp<T, AD<T>>
  {
   protected:
    static thread_local Tape<T> _tape;
    T _value;
    offset_type _index;

   public:
    AD(const AD&) = default;
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
}  // namespace AutoDiffCpp
