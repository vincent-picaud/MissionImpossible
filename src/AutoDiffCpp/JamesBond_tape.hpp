#pragma once

#include <cassert>
#include <cstddef>

namespace AutoDiffCpp
{
  template <typename TAPE>
  class JamesBond_Tape
  {
    friend TAPE;

   public:
    using index_type = typename TAPE::index_type;

   private:
    TAPE& _tape;
    const index_type _index;

    JamesBond_Tape(TAPE& tape) : _tape(tape), _index(_tape.row_size()) {}

    JamesBond_Tape operator=(const JamesBond_Tape&) = delete;

   public:
    ~JamesBond_Tape() { _tape.rewind(_index); }

    std::size_t
    size() const
    {
      assert(_index <= _tape.row_size());
      return _tape.row_size() - _index;
    }
    index_type
    index() const
    {
      return _index;
    };
  };
}
