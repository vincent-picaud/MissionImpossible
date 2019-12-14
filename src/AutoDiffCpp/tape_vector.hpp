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
    using value_type = typename TAPE::value       cftrn,:§_type;

   private:
    const index_type _offset;
    const index_type _size;
    std::shared_array<
  }
}
