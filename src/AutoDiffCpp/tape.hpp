#pragma once

#include "AutoDiffCpp/tape_fwd.hpp"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <type_traits>

namespace AutoDiffCpp
{
  template <typename T>
  class Tape
  {
   public:
    using index_type = tape_index_type;
    using value_type = T;

    struct JamesBond_Mark;

   private:
    struct Index_PartialD
    {
      index_type index;
      value_type value;
    };
    static_assert(std::is_trivially_copyable_v<Index_PartialD>);

   private:
    bool
    check_invariant() const
    {
      bool ok = true;
      ok &= _index_end <= _index_capacity;
      ok &= _index_end > 0;
      ok &= _index[_index_end - 1] <= _tape_capacity;
      return ok;
    }

    template <typename P>
    static inline void
    resize(std::size_t new_size, std::size_t& capacity, P*& p)
    {
      constexpr std::size_t growth_factor = 2;

      if (new_size > capacity)
      {
        new_size = std::max(new_size, growth_factor * capacity);

        const auto new_p = new P[new_size];

        std::memcpy(new_p, p, new_size * sizeof(decltype(*p)));
        capacity = new_size;
        delete[] p;
        p = new_p;
      }
    }
    void
    index_resize(std::size_t new_size)
    {
      resize(new_size, _index_capacity, _index);
    }

    void
    tape_resize(std::size_t new_size)
    {
      resize(new_size, _tape_capacity, _tape);
    };

    [[nodiscard]] Index_PartialD*
    add_row(const std::size_t row_size)
    {
      const auto row_index_begin = _index[_index_end - 1];
      index_resize(_index_end + 1);
      _index[_index_end] = _index[_index_end - 1] + row_size;
      tape_resize(_index[_index_end++]);

      return _tape + row_index_begin;
    }

   public:
    Tape() : Tape(1024, 4 * 1024) {}

    Tape(const std::size_t index_capacity, const std::size_t tape_capacity)
        : _index_end(1),
          _index_capacity(index_capacity),
          _index(new index_type[_index_capacity]),
          _tape_capacity(tape_capacity),
          _tape(new Index_PartialD[tape_capacity])
    {
      assert(_index_capacity > 0);
      _index[0] = 0;
    }

    std::size_t
    row_size() const
    {
      return _index_end - 1;
    };

    std::size_t
    memory_size() const
    {
      return _index_end * sizeof(index_type) + _index[_index_end - 1] * sizeof(Index_PartialD);
    };

    // Old encoding

    // index_type
    // add_variable()
    // {
    //   const std::size_t index = row_size();
    //   auto* const p           = add_row(1);

    //   p->index = index;
    //   p->value = 1;

    //   assert(index <= std::numeric_limits<index_type>::max());
    //   return static_cast<index_type>(index);
    // }

    // New encoding: to best tested in reverse mode!
    index_type
    add_variable()
    {
      const std::size_t index = row_size();
      (void)add_row(0);

      assert(index <= std::numeric_limits<index_type>::max());
      return static_cast<index_type>(index);
    }

    void
    add_row(const std::size_t row_size,
            const index_type* const p_index,
            const value_type* const p_value)
    {
      auto* const p_dest = add_row(row_size);
      for (std::size_t i = 0; i < row_size; i++)
      {
        p_dest[i] = Index_PartialD{p_index[i], p_value[i]};
      }
    }
    template <size_t ROW_SIZE>
    void
    add_row(const std::integral_constant<std::size_t, ROW_SIZE>,
            const index_type* const p_index,
            const value_type* const p_value)
    {
      auto* p_dest = add_row(ROW_SIZE);
      for (std::size_t i = 0; i < ROW_SIZE; ++i)
      {
        p_dest[i] = Index_PartialD{p_index[i], p_value[i]};
      }
    }

    JamesBond_Mark
    JamesBond_mark()
    {
      return {*this};
    }

    void
    rewind(const index_type index_begin)
    {
      assert(index_begin <= row_size());
      _index_end = index_begin + 1;

      assert(check_invariant());
    }

    void
    forward(const std::size_t row_begin, value_type* const diff) const
    {
      const auto row_end = _index_end - 1;
      for (std::size_t i = row_begin; i < row_end; ++i)
      {
        const auto partial_begin = _index[i];
        const auto partial_end   = _index[i + 1];

        for (std::size_t j = partial_begin; j < partial_end; ++j)
        {
          diff[i] += _tape[j].value * diff[_tape[j].index];
        }
      }
    }
    void
    reverse(const std::size_t row_begin, value_type* const diff) const
    {
      const auto row_end = _index_end - 1;
      for (std::size_t i = row_end; i > row_begin; --i)
      {
        const auto partial_begin = _index[i - 1];
        const auto partial_end   = _index[i];
        const auto diff_i_m_1    = diff[i - 1];

        // NOTE: tests
        // - diff_i_m_1 == 0
        // - diff_i_m_1 == +1
        // - diff_i_m_1 == -1
        //
        // are not necessary to make it work, it is only optimization
        // that are relevant for nested case
        //
        // TODO: maybe introduce a if constexpr: for the not nested
        // case, remove the tests that maybe slowdown computation
        // -> TO CHECK. IMPORTANT!
        //
        if (diff_i_m_1 != 0)
        {
          if (diff_i_m_1 == 1)
          {
            for (std::size_t j = partial_begin; j < partial_end; ++j)
            {
              const auto indirection = _tape[j].index;
              diff[indirection] += _tape[j].value;
            }
            continue;
          }

          // does not reduce tape size
          // if (diff_i_m_1 == -1)
          // {
          //   for (std::size_t j = partial_begin; j < partial_end; ++j)
          //   {
          //     const auto indirection = _tape[j].index;
          //     diff[indirection] -= _tape[j].value;
          //   }
          //   continue;
          // }

          // This is the generic that work without the test
          for (std::size_t j = partial_begin; j < partial_end; ++j)
          {
            const auto indirection = _tape[j].index;
            diff[indirection] += _tape[j].value * diff_i_m_1;
          }
        }
      }
    }

    ~Tape()
    {
      delete[] _tape;
      delete[] _index;
    }

   protected:
    std::size_t _index_end;
    std::size_t _index_capacity;
    index_type* _index;

    // NOTE: tape_end is deduced as follows:
    //       tape_end = _index[_index_end - 1]
    //
    std::size_t _tape_capacity;
    Index_PartialD* _tape;

    friend std::ostream&
    operator<<(std::ostream& out, const Tape& to_print)
    {
      for (size_t i = 0; i + 1 < to_print._index_end; i++)
      {
        out << std::setw(5) << i << " ";

        if (to_print._index[i] == to_print._index[i + 1])
        {
          out << std::endl << std::endl;
          continue;
        }

        for (size_t j = to_print._index[i]; j < to_print._index[i + 1]; j++)
        {
          out << "[" << std::setw(5) << to_print._tape[j].index << ", " << std::setw(15)
              << to_print._tape[j].value << "]" << std::endl;

          if (j + 1 != to_print._index[i + 1])
          {
            out << std::setw(5) << " "
                << " ";
          }
          else
          {
            out << std::endl;
          }
        }
      }
      return out;
    }
  };

  /////////////////////////////
  // Define internal objects //
  /////////////////////////////

  template <typename T>
  struct Tape<T>::JamesBond_Mark
  {
    friend Tape<T>;

   private:
    Tape<T>& _tape;
    const index_type _index_begin;

    JamesBond_Mark(Tape<T>& tape) : _tape(tape), _index_begin(_tape.row_size()) {}

    JamesBond_Mark operator=(const JamesBond_Mark&) = delete;

   public:
    ~JamesBond_Mark() { _tape.rewind(_index_begin); }

    std::size_t
    size() const
    {
      assert(_tape.row_size() >= _index_begin);
      return _tape.row_size() - _index_begin;
    }
    index_type
    index_begin() const
    {
      return _index_begin;
    };
  };

  ///////////////////////////
  // Tape Singleton Access //
  ///////////////////////////

  template <typename T>
  inline Tape<T>&
  tape()
  {
    thread_local static Tape<T> _tape;

    return _tape;
  }

}  // namespace AutoDiffCpp
