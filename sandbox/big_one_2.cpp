// somes test a new implementation with the aim of being able to have
// delayed tape registration not only for first order but also high
// order derivative

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

template <typename... COEF_i>
struct AD_Differential_Tuple
{
  std::tuple<COEF_i...> _value;
};
template <typename T, std::size_t N>
struct AD_Differential_Array
{
 protected:
  static_assert(std::is_same_v<T, double>);
  std::array<T, N> _value;
  std::array<std::size_t, N> _index;

 public:
  AD_Differential_Array(const std::array<T, N>& value, const std::array<std::size_t, N>& index)
      : _value(value), _index(index)
  {
  }

  const std::array<T, N>&
  value() const
  {
    return _value;
  }
  const std::array<std::size_t, N>&
  index() const
  {
    return _index;
  }
};

template <typename T>
auto
create_differential(const T value, const std::size_t index)
{
  return AD_Differential_Array<T, 1>{{value}, {index}};
}

//////////////////////////////////////////////////////////////////

template <typename T, typename AD_DIFFERENTIAL>
struct AD_Function;

// Final
template <typename T, size_t N>
struct AD_Function<T, AD_Differential_Array<T, N>>
{
  // T is a basic type like float, double!
  //  static_assert(std::is_same_v<T, AD_Variable_Final_Value_Type_t<T>>);
  static_assert(std::is_same_v<T, double>);  // parano!

  using differential_type = AD_Differential_Array<T, N>;
  using value_type        = T;

 protected:
  value_type _value;
  differential_type _df;

 public:
  AD_Function(){};
  AD_Function(const value_type value, const differential_type& df) : _value(value), _df(df) {}

  value_type
  value() const
  {
    return _value;
  }
  const differential_type&
  df() const
  {
    return _df;
  }

  friend std::ostream&
  operator<<(std::ostream& out, const AD_Function& to_print)
  {
    out << "f=" << to_print._value << std::endl;
    return out;
  }
};

// A function of some variable
template <typename T, typename... COEF>
struct AD_Function<AD_Variable<T>, AD_Differential_Tuple<T, COEF...>>
{
  using value_type        = AD_Variable_Final_Value_Type_t<T>;
  using differential_type = AD_Differential_Tuple<COEF...>;

 protected:
  value_type _f_value;
  // AD_Differential must be AD_Differential_Crtp<T,...>
  differential_type _df_value;

 public:
  AD_Function(){};
  AD_Function(const value_type f, const differential_type& df) : _f_value(f), _df_value(df) {}

  value_type
  value() const
  {
    return _f_value;
  }
  const differential_type&
  df() const
  {
    return _df_value;
  }

  // friend std::ostream&
  // operator<<(std::ostream& out, const AD_Function& to_print)
  // {
  //   out << " f_value: " << to_print._f_value << std::endl;
  //   out << "df_value: " << to_print._df_value << std::endl;
  //   return out;
  // }
};

//////////////////////////////////////////////////////////////////
// template <typename T>
// auto operator*(const AD_Variable_Final_Value_Type_t<T> x0, const AD_Variable<T>& x1)
// {
//   return create_function(x0 * x1.final_value(),
//                          std::make_tuple(AD_Differential_Coef<T>(1, x1.index())));
// }

// template <typename T>
// auto operator*(const AD_Variable<T>& x0,
//                const AD_Variable<T>& x1)
// {
//   return create_function(x0.final_value() * x1.final_value(),create_differential(x0.value()*;
// }

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

void
test_0()
{
  reset_global_index();
  std::cout << "===> " << __PRETTY_FUNCTION__ << std::endl;

  using T = double;

  AD_Variable<T> x0{2};
  3 * x0;

  // create_function(x0);
  // auto y = x0 * x0;
  // std::cout << y << std::endl;
}
void
test_1()
{
  reset_global_index();
  std::cout << "===> " << __PRETTY_FUNCTION__ << std::endl;

  using T = double;

  AD_Variable<AD_Variable<T>> x0{2};
  //  create_function(x0);
  // auto y = x0 * x0;
  // std::cout << y << std::endl;
}
int
main()
{
}
