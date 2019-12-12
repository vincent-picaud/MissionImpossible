#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <type_traits>

namespace AutoDiffCpp
{
  using offset_type = unsigned int;

  template <typename T>
  struct Index_PartialDiff
  {
    offset_type index;
    T value;
  };

  template <typename T>
  class Tape
  {
    static_assert(std::is_trivial_v<Index_PartialDiff<T>>);

    bool check_invariant() const
    {
      bool ok = true;
      ok &= _offset_end <= _offset_capacity;
      ok &= _offset_end > 0;
      ok &= _offset[_offset_end - 1] <= _tape_capacity;
      return ok;
    }

   protected:
    template <typename P>
    static inline void resize(std::size_t new_size, std::size_t& capacity, P*& p)
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
    void offset_resize(std::size_t new_size)
    {
      resize(new_size, _offset_capacity, _offset);
    }

    void tape_resize(std::size_t new_size)
    {
      resize(new_size, _tape_capacity, _tape);
    };

    [[nodiscard]] Index_PartialDiff<T>* add_row(const std::size_t row_size)
    {
      const auto row_offset_begin = _offset[_offset_end - 1];
      offset_resize(_offset_end + 1);
      _offset[_offset_end] = _offset[_offset_end - 1] + row_size;
      tape_resize(_offset[_offset_end++]);

      return _tape + row_offset_begin;
    }

   public:
    Tape() : Tape(1024, 4 * 1024)
    {
    }
    Tape(const std::size_t offset_capacity, const std::size_t tape_capacity)
        : _offset_end(1)
        , _offset_capacity(offset_capacity)
        , _offset(new offset_type[_offset_capacity])
        , _tape_capacity(tape_capacity)
        , _tape(new Index_PartialDiff<T>[tape_capacity])
    {
      assert(_offset_capacity > 0);
      _offset[0] = 0;
    }

    std::size_t row_size() const
    {
      return _offset_end - 1;
    };

    void add_row(const std::size_t row_size, const offset_type* const p_offset, const T* const p_T)
    {
      auto* const p_dest = add_row(row_size);
      for (std::size_t i = 0; i < row_size; i++)
      {
        p_dest[i] = Index_PartialDiff<T>{p_offset[i], p_T[i]};
      }
    }
    template <size_t ROW_SIZE>
    void add_row(const std::integral_constant<std::size_t, ROW_SIZE>, const offset_type* const p_offset, const T* const p_T)
    {
      auto* p_dest = add_row(ROW_SIZE);
      for (std::size_t i = 0; i < ROW_SIZE; ++i)
      {
        p_dest[i] = Index_PartialDiff<T>{p_offset[i], p_T[i]};
      }
    }

    void forward(const std::size_t row_begin, T* const diff)
    {
      const auto row_end = _offset_end - 1;
      for (std::size_t i = row_begin; i < row_end; ++i)
      {
        auto* const partial_begin = _offset[i];
        auto* const partial_end = _offset[i + 1];

        T sum = 0;
        for (std::size_t j = partial_begin; j < partial_end; ++j)
        {
          sum += _tape[i].value * diff[_tape[i].index];
        }
        diff[i] = sum;
      }
    }

    ~Tape()
    {
      delete[] _tape;
      delete[] _offset;
    }

   protected:
    std::size_t _offset_end;
    std::size_t _offset_capacity;
    offset_type* _offset;

    // NOTE: tape_end is deduced as follows:
    //       tape_end = _offset[_offset_end - 1]
    //
    std::size_t _tape_capacity;
    Index_PartialDiff<T>* _tape;

    friend std::ostream& operator<<(std::ostream& out, const Tape& to_print)
    {
      for (size_t i = 0; i + 1 < to_print._offset_end; i++)
      {
        out << std::setw(5) << i << " ";
        for (size_t j = to_print._offset[i]; j < to_print._offset[i + 1]; j++)
        {
          out << "[" << std::setw(5) << to_print._tape[j].index << ", " << std::setw(15) << to_print._tape[j].value << "]" << std::endl;

          if (j + 1 != to_print._offset[i + 1])
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
