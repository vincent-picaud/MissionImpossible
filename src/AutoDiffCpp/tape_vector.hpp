#pragma once

#include <memory>

namespace AutoDiffCpp
{
  template <typename TAPE>
  class Tape_Vector
  {
    friend TAPE;

   public:
    using index_type = typename TAPE::index_type;
    using value_type = typename TAPE::value;

   private:
    //  const index_type _offset;
    const index_type _size;
    std::shared_array<value_type[]> _data;


  public:
    Tape_Vector(const Tape& t
  }
}
