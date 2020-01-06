#pragma once

#include "MissionImpossible/ad.hpp"
#include "MissionImpossible/mission_impossible_tape.hpp"
#include "MissionImpossible/tape.hpp"

#include <iomanip>
#include <iostream>
#include <memory>

namespace MissionImpossible
{
  template <typename T>
  class Tape_Vector
  {
   private:
    std::size_t _offset;
    std::size_t _size;
    // IMPORTANT:
    // principe of least surprise,  _data[i] must be valid for 0 <= i < size
    // CAVEAT: op[AD<T>x] ok, but internally use [x.index()-offset]
    std::shared_ptr<T[]> _data;

    bool
    check_AD_index(const std::size_t i) const
    {
      return (i >= _offset) and (i - _offset < _size);
    }
    bool
    check_index(const std::size_t i) const
    {
      return i < _size;
    }

   public:
    Tape_Vector(const Tape<T>& tape) : _offset(0), _size(tape.row_size()), _data(new T[_size]) {}
    Tape_Vector(const Mission_Impossible_Tape<T>& tape)
        : _offset(tape.index_begin()), _size(tape.size()), _data(new T[_size])
    {
    }

    // data[i], 0 <= i < size
    const T*
    data() const
    {
      return _data.get();
    }
    T*
    data()
    {
      return _data.get();
    }

    std::size_t
    size() const
    {
      return _size;
    };

    void
    assign_ei(const std::size_t i_star)
    {
      assert(check_index(i_star));

      auto* p = _data.get();
      for (std::size_t i = 0; i < _size; ++i)
      {
        p[i] = 0;
      }
      p[i_star] = 1;
    }
    void
    assign_ei(const AD<T>& var_star)
    {
      const auto i_star = var_star.index();
      assert(check_AD_index(i_star));
      assign_ei(i_star - _offset);
    }

    const T& operator[](const AD<T>& var) const
    {
      const auto i = var.index();
      assert(check_AD_index(i));
      return data()[i - _offset];
    }
    // Syntaxic sugar for nested case
    //
    // Allows:
    //
    // Hessian_row[x]
    //
    // intead of
    //
    // Hessian_row[x.value()]
    //
    template <typename U, typename = std::enable_if_t<Is_AD_v<U>>>
    const auto& operator[](const AD<U>& var) const
    {
      return (*this)[var.value()];
    }

    T& operator[](const AD<T>& var)
    {
      const auto i = var.index();
      assert(check_AD_index(i));
      return data()[i - _offset];
    }
    // Syntaxic sugar for nested case
    template <typename U, typename = std::enable_if_t<Is_AD_v<U>>>
    auto& operator[](const AD<U>& var)
    {
      return (*this)[var.value()];
    }

    const T& operator[](const typename AD<T>::index_type i) const
    {
      assert(check_index(i));
      return data()[i];
    }
    T& operator[](const typename AD<T>::index_type i)
    {
      assert(check_index(i));
      return data()[i];
    }

    friend std::ostream&
    operator<<(std::ostream& out, const Tape_Vector<T>& to_print)
    {
      for (size_t i = 0; i < to_print.size(); ++i)
      {
        out << std::setw(5) << i + to_print._offset << "\t" << std::setw(5) << i
            << "\t: " << to_print[i] << std::endl;
      }
      return out;
    }
  };
}
