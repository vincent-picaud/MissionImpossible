#pragma once

#include "Mission_Impossible_AutoDiff/ad_fwd.hpp"
#include "Mission_Impossible_AutoDiff/tape.hpp"

namespace Mission_Impossible_AutoDiff
{
  namespace Detail
  {
    template <typename T>
    class Mission_Impossible_Tape_Data
    {
     public:
      using index_type = typename Tape<T>::index_type;

     protected:
      const index_type _index_begin;

      Tape<T>&
      tape() const noexcept
      {
        return Mission_Impossible_AutoDiff::tape<T>();
      }

      Mission_Impossible_Tape_Data() : _index_begin(tape().row_size()) {}

     public:
      ~Mission_Impossible_Tape_Data() { tape().rewind(_index_begin); }

      std::size_t
      size() const
      {
        assert(tape().row_size() >= _index_begin);
        return tape().row_size() - _index_begin;
      }
      index_type
      index_begin() const
      {
        return _index_begin;
      };
    };
  }

  // Recursive implementation to smoothly support nested AD<AD<...>>
  //
  template <typename T>
  class Mission_Impossible_Tape;

  template <typename T>
  class Mission_Impossible_Tape : public Detail::Mission_Impossible_Tape_Data<T>
  {
   protected:
    using base_type = Detail::Mission_Impossible_Tape_Data<T>;

   public:
    Mission_Impossible_Tape() : base_type() {}
  };

  template <typename T>
  class Mission_Impossible_Tape<AD<T>> : public Mission_Impossible_Tape<T>,
                                         public Detail::Mission_Impossible_Tape_Data<AD<T>>
  {
   protected:
    using recursion_type = Mission_Impossible_Tape<T>;
    using base_type      = Detail::Mission_Impossible_Tape_Data<AD<T>>;

   public:
    using base_type::index_begin;
    using base_type::size;
    using base_type::tape;

    Mission_Impossible_Tape() : recursion_type(), base_type() {}
  };

}
