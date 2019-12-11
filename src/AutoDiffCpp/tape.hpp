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

   public:
    Tape() : Tape(1024, 4 * 1024) {}
    Tape(const std::size_t offset_capacity, const std::size_t tape_capacity)
        : _offset_position(0)
        , _offset_capacity(offset_capacity)
        , _offset(new offset_type[_offset_capacity])
        , _tape_capacity(tape_capacity)
        , _tape(new Index_PartialDiff<T>[tape_capacity])
    {
      assert(_offset_capacity > 0);
      _offset[_offset_position++] = 0;
    }

    std::size_t offset_capacity() const { return _offset_capacity; };

    void offset_resize(const std::size_t size) const
    {
      if (size > offset_capacity())
      {
        const auto new_offset = new offset_type[size];

        std::memcpy(new_offset, _offset, size * sizeof(offset_type));
        _offset_capacity = size;
        _offset = new_offset;
      }
    }

    std::size_t tape_size() const { return _offset[_offset_position]; };
    std::size_t tape_capacity() const { return _tape_capacity; };

    void add_row(const std::size_t row_size) {}

    ~Tape()
    {
      delete[] _tape;
      delete[] _offset;
    }

   protected:
    std::size_t _offset_position;
    std::size_t _offset_capacity;
    offset_type* _offset;

    // note: tape position is deduced from offset last element
    std::size_t _tape_capacity;
    Index_PartialDiff<T>* _tape;

  };  // namespace AutoDiffCpp

}  // namespace AutoDiffCpp
