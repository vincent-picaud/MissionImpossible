#pragma once

#include "AutoDiffCpp/ad_fwd.hpp"
#include "AutoDiffCpp/tape.hpp"

namespace AutoDiffCpp
{
  namespace Detail
  {
    template <typename T>
    class Mission_Impossible_Tape_Base
    {
     public:
      using index_type = typename Tape<T>::index_type;

     protected:
      const index_type _index_begin;

      Tape<T>&
      tape() const noexcept
      {
        return AutoDiffCpp::tape<T>();
      }

      Mission_Impossible_Tape_Base() : _index_begin(tape().row_size()) {}

      Mission_Impossible_Tape_Base operator=(const Mission_Impossible_Tape_Base&) = delete;

     public:
      ~Mission_Impossible_Tape_Base() { tape().rewind(_index_begin); }

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

  // First order case
  template <typename T>
  struct Mission_Impossible_Tape : public Detail::Mission_Impossible_Tape_Base<T>
  {
   protected:
    using base_type = Detail::Mission_Impossible_Tape_Base<T>;

   public:
    Mission_Impossible_Tape() : base_type() {}
  };

  // Nested tape cases
  template <typename T>
  struct Mission_Impossible_Tape<AD<T>> : public Detail::Mission_Impossible_Tape_Base<AD<T>>
  {
   protected:
    Mission_Impossible_Tape<T> _recursive;

   protected:
    using base_type = Detail::Mission_Impossible_Tape_Base<AD<T>>;

   public:
    Mission_Impossible_Tape() : base_type(), _recursive() {}
    operator const Mission_Impossible_Tape<T>&() const noexcept { return _recursive; };
  };
}
