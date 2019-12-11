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
   public:
    Tape() : Tape(1024, 4 * 1024) {}
    Tape(std::size_t offset_capacity, std::size_t tape_capacity),
        _offset_position(0), _offset_capacity(offset_capacity),
        _offset(new offset_type[_offset_capacity)]),
        _tape_capacity(tape_capacity),  _tape(new Index_PartialDiff[tape_capacity])
    {
      assert(_offset_capacity > 0);
      offset_[_offset_position++] = 0;
    }

    void offset_resize(const std::size_t size) const
    {
      if (size > _offset_capacity)
      {
        _offset_capacity = size;
        const auto new_offset = new offset_type[_offset_capacity)];

        std::copy(_offset, _offsert + _offset_position, new_offset);
        _offset = new_offset;
      }
    }

    void add_row(const std::size_t row_size) {}

    offset ~Tape()
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
    Index_PartialDiff* _tape;

  };  // namespace AutoDiffCpp

}  // namespace AutoDiffCpp
