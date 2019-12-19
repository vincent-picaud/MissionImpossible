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

// template <typename... COEF_i>
// struct AD_Differential_Tuple
// {
//  protected:
//   std::tuple<COEF_i...> _value;

//  public:
//   AD_Differential_Tuple(const std::tuple<COEF_i...>& value) : _value(value) {}

//   const std::tuple<COEF_i...>&
//   value() const
//   {
//     return _value;
//   };
// };
template <class Ch, class Tr, class... Args>
auto&
operator<<(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t)
{
  std::apply([&os](auto&&... args) { ((os << args << " "), ...); }, t);
  return os;
}
//std::ostream& operator<<(std::ostream&out,std::tuple<C0,Ci...>
template <typename T>
auto
create_differential(const AD_Variable_Final_Value_Type_t<T> value,
                    const AD_Variable<AD_Variable<T>>& x)
{
  return std::make_tuple(
      create_function(value, create_differential(AD_Variable_Final_Value_Type_t<T>(1), x.value())));
}

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
create_differential(const T value, const AD_Variable<T>& x)
{
  return AD_Differential_Array<T, 1>{{value}, {x.index()}};
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
    out << "f=" << to_print._value << "\tdf=[";
    for (size_t i = 0; i < N; ++i)
    {
      out << "d" << to_print._df.index()[i] << "=" << to_print._df.value()[i] << ", ";
    }
    out << "]";
    return out;
  }
};

template <typename T, size_t N>
auto
create_function(const T f, const AD_Differential_Array<T, N>& df)
{
  return AD_Function<T, AD_Differential_Array<T, N>>(f, df);
}

// A function of some variable
template <typename T, typename... COEF>
struct AD_Function<T, std::tuple<COEF...>>
{
  static_assert(std::is_same_v<T, double>);  // parano!

  using value_type        = T;
  using differential_type = std::tuple<COEF...>;

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

  friend std::ostream&
  operator<<(std::ostream& out, const AD_Function& to_print)
  {
    out << "f=" << to_print.value() << "\tdf=[" << to_print.df() << "]";

    return out;
  }
};

template <typename T, typename... COEF>
auto
create_function(const T f, const std::tuple<COEF...>& df)
{
  return AD_Function<T, std::tuple<COEF...>>(f, df);
}

//////////////////////////////////////////////////////////////////

template <typename T>
auto operator*(const AD_Variable_Final_Value_Type_t<T> x0, const AD_Variable<T>& x1)
{
  return create_function(x0 * x1.value(), create_differential(T(1), x1));
}
template <typename T>
auto operator*(const AD_Variable_Final_Value_Type_t<T> x0, const AD_Variable<AD_Variable<T>>& x1)
{
  return create_function(x0 * x1.final_value(),
                         create_differential(AD_Variable_Final_Value_Type_t<T>(1), x1));
}

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
  auto y = 3 * x0;
  std::cout << y;
  // create_function(x0);
  // auto y = x0 * x0;
  // std::cout << y << std::endl;
}
void
test_1()
{
  reset_global_index();
  std::cout << "\n\n===> " << __PRETTY_FUNCTION__ << std::endl;

  using T = double;

  AD_Variable<AD_Variable<T>> x0{2};
  auto y = 3 * x0;
  std::cout << y << std::endl;

  AD_Variable<AD_Variable<AD_Variable<T>>> x1{2};
  auto y2 = 3 * x1;
  std::cout << y2 << std::endl;
  //  create_function(x0);
  // auto y = x0 * x0;
  // std::cout << y << std::endl;
}
int
main()
{
  test_0();
  test_1();
}
