#pragma once

#include "AutoDiffCpp/JamesBond_tape.hpp"

#include <memory>

namespace AutoDiffCpp
{
  template <typename TAPE>
  class Tape_Vector
  {
   public:
    using index_type = typename TAPE::index_type;
    using value_type = typename TAPE::value_type;

   private:
    const index_type _offset;  // a priori only used in assert
    const index_type _size;
    std::shared_ptr<value_type[]> _data;

   public:
    Tape_Vector(const JamesBond_Tape<TAPE>& jb_tape)
        : _offset(jb_tape.index()), _size(jb_tape.size()), _data(new value_type[_size])
    {
      _data = std::shared_ptr<value_type[]>(_data, _data.get() - jb_tape.index());
    }

    std::size_t
    size() const
    {
      return _size;
    };
    value_type& operator[](const std::size_t i)
    {
      assert((i > _offset) and (i - _offset < _size));
      return _data.get()[i];
    }
  };
}
