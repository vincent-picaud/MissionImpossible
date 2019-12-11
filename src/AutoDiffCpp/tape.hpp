#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstring>
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
      ok &= _tape_end <= _tape_capacity;
      ok &= _offset_end > 0;
      ok &= _tape_end == _offset[_offset_end - 1];
      return ok;
    }

   protected:
    template <typename P>
    static inline void resize(std::size_t new_size,
                              std::size_t& capacity,
                              P*& p)
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

   public:
    Tape() : Tape(1024, 4 * 1024) {}
    Tape(const std::size_t offset_capacity, const std::size_t tape_capacity)
        : _offset_end(1)
        , _offset_capacity(offset_capacity)
        , _offset(new offset_type[_offset_capacity])
        , _tape_end(0)
        , _tape_capacity(tape_capacity)
        , _tape(new Index_PartialDiff<T>[tape_capacity])
    {
      assert(_offset_capacity > 0);
      _offset[0] = 0;
    }

    // std::size_t offset_capacity() const { return _offset_capacity; };

    void offset_resize(std::size_t new_size)
    {
      resize(new_size, _offset_capacity, _offset);
    }

    // std::size_t tape_size() const { return _tape_end; };
    // std::size_t tape_capacity() const { return _tape_capacity; };

    void tape_resize(std::size_t new_size)
    {
      resize(new_size, _tape_capacity, _tape);
    };

    [[nodiscard]] Index_PartialDiff<T>* add_row(const std::size_t row_size)
    {
      assert(_offset[_offset_end - 1] == _tape_end);

      const auto row_offset_begin = _tape_end;
      _tape_end += row_size;
      tape_resize(_tape_end);

      offset_resize(_offset_end + 1);
      _offset[_offset_end++] = _tape_end;

      return _tape + row_offset_begin;
    }

    std::size_t row_size() const { return _offset_end - 1; };

    ~Tape()
    {
      delete[] _tape;
      delete[] _offset;
    }

   protected:
    std::size_t _offset_end;
    std::size_t _offset_capacity;
    offset_type* _offset;

    // NOTE: tape position cans deduced from offset last element (see
    // check_invariant()), however it is faster to have a direct
    // access
    std::size_t _tape_end;
    std::size_t _tape_capacity;
    Index_PartialDiff<T>* _tape;

  };  // namespace AutoDiffCpp

}  // namespace AutoDiffCpp
