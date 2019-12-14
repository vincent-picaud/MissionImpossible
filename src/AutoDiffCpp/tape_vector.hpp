#pragma once

#include "AutoDiffCpp/JamesBond_tape.hpp"
#include "AutoDiffCpp/ad.hpp"

#include <memory>

namespace AutoDiffCpp
{
  // TODO: certainly an artificial dependency on tape, TOFIX
  //
  template <typename TAPE>
  class Tape_Vector
  {
    friend TAPE;

   public:
    using index_type = typename TAPE::index_type;
    using value_type = typename TAPE::value_type;

   private:
    const index_type _offset;  // a priori only used in assert
    const index_type _size;
    std::shared_ptr<value_type[]> _data;

    // CAVEAT:
    // data()[i] only makes sense when: (i > _offset) and (i - _offset < _size)
    // -> this method is essentially used by Tape.
    const value_type*
    data() const
    {
      return _data.get();
    }
    value_type*
    data()
    {
      return _data.get();
    }

    bool
    check_index(const std::size_t i) const
    {
      return (i > _offset) and (i - _offset < _size);
    }

   public:
    Tape_Vector(const TAPE& tape) : _offset(0), _size(tape.row_size()), _data(new value_type[_size])
    {
    }
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

    void
    assign_ei(const std::size_t i_star)
    {
      assert(check_index(i_star));

      auto* p = _data.get() + _offset;
      for (std::size_t i = 0; i < _size; ++i)
      {
        p[i] = 0;
      }
      p[i_star] = 1;
    }
    const value_type& operator[](const AD<value_type>& var) const
    {
      const auto i = var.index();
      assert(check_index(i));
      return _data.get()[i];
    }
    value_type& operator[](const AD<value_type>& var)
    {
      const auto i = var.index();
      assert(check_index(i));
      return _data.get()[i];
    }
  };
}
