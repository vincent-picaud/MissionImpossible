
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

//////////////////////////////////////////////////////////////////

template <typename T, typename DIFFERENTIAL>
struct AD_Function;

// Store a differential with components:
//
//   ---------
//   | COEF  |
// < | index | .... >
//   ---------
//
// Here as COEF can be function of different types, we use a tuple
//
// Test:
// - test_0()
//
template <typename... COEFs>
struct AD_Differential_Tuple
{
 public:
  using index_type       = std::size_t;
  using value_array_type = std::tuple<COEFs...>;
  using index_array_type = std::array<index_type, sizeof...(COEFs)>;

 protected:
  value_array_type _value_array;
  index_array_type _index_array;

 public:
  AD_Differential_Tuple(const value_array_type& value_array, const index_array_type& index_array)
      : _value_array(value_array), _index_array(index_array)
  {
  }

  const std::tuple<COEFs...>&
  value() const
  {
    return _value_array;
  };
  const std::tuple<COEFs...>&
  index() const
  {
    return _index_array;
  };

  friend auto&
  operator<<(std::ostream& out, const AD_Differential_Tuple& to_print)
  {
    std::size_t i = 0;
    out << "[ ";
    std::apply(
        [&out, &i, &to_print](auto&&... args) {
          ((out << "d" << to_print._index_array[i++] << "=" << args << ", "), ...);
        },
        to_print.value());
    out << "]";
    return out;
  }
};

//================================================================

// The same but with array (when COEF are all scalars)
//
// Test:
// - test_1()
//
template <typename T, std::size_t N>
struct AD_Differential_Array
{
  static_assert(std::is_same_v<T, double>);

 public:
  using index_type       = std::size_t;
  using value_array_type = std::array<T, N>;
  using index_array_type = std::array<std::size_t, N>;

 protected:
  std::array<T, N> _value_array;
  std::array<std::size_t, N> _index_array;

 public:
  AD_Differential_Array(){};
  AD_Differential_Array(const value_array_type& value_array, const index_array_type& index_array)
      : _value_array(value_array), _index_array(index_array)
  {
  }

  const value_array_type&
  value() const
  {
    return _value_array;
  }
  const index_array_type&
  index() const
  {
    return _index_array;
  }

  friend auto&
  operator<<(std::ostream& out, const AD_Differential_Array& to_print)
  {
    out << "[ ";
    for (std::size_t i = 0; i < N; i++)
    {
      out << "d" << to_print._index_array[i] << "=" << to_print._index_array[i] << ", ";
    }
    out << "]";
    return out;
  }
};

//////////////////////////////////////////////////////////////////
// Helpers
//////////////////////////////////////////////////////////////////
void
reset_global_index()
{
  using T = double;

  AD_Variable<T>::global_index                           = 0;
  AD_Variable<AD_Variable<T>>::global_index              = 0;
  AD_Variable<AD_Variable<AD_Variable<T>>>::global_index = 0;
}

template <typename T>
void
printType(const T&)
{
  std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

//////////////////////////////////////////////////////////////////
// Tests
//////////////////////////////////////////////////////////////////
void
test_0()
{
  reset_global_index();
  std::cout << "\n\n===> " << __PRETTY_FUNCTION__ << std::endl;

  AD_Differential_Tuple<double, double> diff(std::make_tuple(4., 5.), {0, 1});

  std::cout << diff;
}
void
test_1()
{
  reset_global_index();
  std::cout << "\n\n===> " << __PRETTY_FUNCTION__ << std::endl;

  AD_Differential_Array<double, 3> diff({4., 5., 6.}, {0, 1, 2});

  std::cout << diff;
}

int
main()
{
  test_0();
  test_1();
}
