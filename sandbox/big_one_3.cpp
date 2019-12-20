
#include <array>
#include <iostream>
#include <tuple>

//////////////////////////////////////////////////////////////////

template <typename T>
struct Always_True : std::true_type
{
};

//================================================================

template <typename T>
struct AD_Variable;

template <typename T, typename ENABLE = void>
struct AD_Variable_Final_Value_Type
{
  using value_type = T;
};

template <typename T>
struct AD_Variable_Final_Value_Type<
    AD_Variable<T>,
    std::enable_if_t<Always_True<typename AD_Variable<T>::value_type>::value>>
{
  using value_type =
      typename AD_Variable_Final_Value_Type<typename AD_Variable<T>::value_type>::value_type;
};

template <typename T>
using AD_Variable_Final_Value_Type_t = typename AD_Variable_Final_Value_Type<T>::value_type;

//////////////////////////////////////////////////////////////////

// Used in conjunction with tape
// Variable
template <typename T>
struct AD_Variable
{
  using value_type = T;
  using index_type = std::size_t;

 protected:
  value_type _value;
  index_type _index;

 public:
  AD_Variable(){};
  AD_Variable(AD_Variable_Final_Value_Type_t<T> value) : _value(value), _index(global_index++) {}

  const AD_Variable_Final_Value_Type_t<T>&
  final_value() const
  {
    if constexpr (std::is_same_v<T, AD_Variable_Final_Value_Type_t<T>>)
    {
      return _value;
    }
    else
    {
      return _value.final_value();
    }
  }
  const value_type&
  value() const
  {
    return _value;
  }
  const index_type&
  index() const
  {
    return _index;
  }

  // For simulation index counter
  static std::size_t global_index;

  friend std::ostream&
  operator<<(std::ostream& out, const AD_Variable& to_print)
  {
    out << to_print._value << "_" << to_print._index;
    return out;
  }
};
template <typename T>
std::size_t AD_Variable<T>::global_index = 0;

int
main()
{
}
