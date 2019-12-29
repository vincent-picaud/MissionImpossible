#pragma once

#include "AutoDiffCpp/ad.hpp"

#include <iomanip>
#include <iostream>
#include <memory>

namespace AutoDiffCpp
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
    Tape_Vector(const std::size_t size) : _offset(0), _size(size), _data(new T[_size]) {}

    // TODO: to keep until moved into another place
    // Tape_Vector(const typename TAPE::JamesBond_Mark& jb_tape)
    //     : _offset(jb_tape.index_begin()), _size(jb_tape.size()), _data(new T[_size])
    // {
    // }

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

    const T& operator[](const AD<T>& var) const
    {
      const auto i = var.index();
      assert(check_AD_index(i));
      return data()[i - _offset];
    }
    T& operator[](const AD<T>& var)
    {
      const auto i = var.index();
      assert(check_AD_index(i));
      return data()[i - _offset];
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
