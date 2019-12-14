#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <type_traits>

namespace AutoDiffCpp
{
  template <typename T>
  struct Index_PartialD
  {
    using index_type = unsigned int;
    using value_type = T;

    index_type index;
    value_type value;
  };

  template <typename T>
  class Tape
  {
    static_assert(std::is_trivial_v<Index_PartialD<T>>);

   public:
    using index_partialD_type = Index_PartialD<T>;
    using index_type          = typename index_partialD_type::index_type;
    using value_type          = typename index_partialD_type::value_type;
    static_assert(std::is_same_v<T, value_type>);

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

    [[nodiscard]] index_partialD_type*
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
          _tape(new index_partialD_type[tape_capacity])
    {
      assert(_index_capacity > 0);
      _index[0] = 0;
    }

    std::size_t
    row_size() const
    {
      return _index_end - 1;
    };
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

    index_type
    add_variable()
    {
      const std::size_t index = row_size();
      (void)add_row(0);

      // p->index = index;
      // p->value = 1;

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
        p_dest[i] = index_partialD_type{p_index[i], p_value[i]};
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
        p_dest[i] = index_partialD_type{p_index[i], p_value[i]};
      }
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
          diff[i] += _tape[i].value * diff[_tape[i].index];
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
    index_partialD_type* _tape;

    friend std::ostream&
    operator<<(std::ostream& out, const Tape& to_print)
    {
      for (size_t i = 0; i + 1 < to_print._index_end; i++)
      {
        out << std::setw(5) << i << " ";

        if (to_print._index[i] == to_print._index[i + 1])
        {
          out << std::endl<< std::endl;
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

}  // namespace AutoDiffCpp
