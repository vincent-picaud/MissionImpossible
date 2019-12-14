#pragma once

#include "AutoDiffCpp/JamesBond_tape.hpp"

#include <memory>

namespace AutoDiffCpp
{
  template <typename TAPE>
  class Tape_Vector
  {
    friend TAPE;  // gives write mode access to tape

   public:
    using index_type = typename TAPE::index_type;
    using value_type = typename TAPE::value_type;

   private:
    const index_type _offset;  // a priori only used in assert
    const index_type _size;
    std::shared_ptr<value_type[]> _data;

    value_type*
    data()
    {
      return _data.get();
    }

   public:
    Tape_Vector(const JamesBond_Tape<TAPE>& jb_tape)
        : _offset(jb_tape.index()), _size(jb_tape.size()), _data(new value_type[_size])
    {
      _data = std::shared_ptr<value_type[]>(_data, _data.get() - jb_tape.index());
    }

    // all public methods are read only
    std::size_t
    size() const
    {
      return _size;
    };

    const value_type& operator[](const std::size_t i) const
    {
      assert((i > _offset) and (i - _offset < _size));
      return _data.get()[i];
    }
    const value_type*
    data() const
    {
      return _data.get();
    }
  };
}
