#pragma once

#include "MissionImpossible/ad_fwd.hpp"
#include "MissionImpossible/tape.hpp"

namespace MissionImpossible
{
  namespace Detail
  {
    template <typename T>
    class MissionImpossible_Tape_Data
    {
     public:
      using index_type = typename Tape<T>::index_type;

     protected:
      const index_type _index_begin;

      Tape<T>&
      tape() const noexcept
      {
        return MissionImpossible::tape<T>();
      }

      MissionImpossible_Tape_Data() : _index_begin(tape().statement_size()) {}

     public:
      ~MissionImpossible_Tape_Data() { tape().rewind(_index_begin); }

      std::size_t
      size() const
      {
        assert(tape().statement_size() >= _index_begin);
        return tape().statement_size() - _index_begin;
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
  class MissionImpossible_Tape;

  template <typename T>
  class MissionImpossible_Tape : public Detail::MissionImpossible_Tape_Data<T>
  {
   protected:
    using base_type = Detail::MissionImpossible_Tape_Data<T>;

   public:
    MissionImpossible_Tape() : base_type() {}
  };

  template <typename T>
  class MissionImpossible_Tape<AD<T>> : public MissionImpossible_Tape<T>,
                                        public Detail::MissionImpossible_Tape_Data<AD<T>>
  {
   protected:
    using recursion_type = MissionImpossible_Tape<T>;
    using base_type      = Detail::MissionImpossible_Tape_Data<AD<T>>;

   public:
    using base_type::index_begin;
    using base_type::size;
    using base_type::tape;

    MissionImpossible_Tape() : recursion_type(), base_type() {}
  };

}
