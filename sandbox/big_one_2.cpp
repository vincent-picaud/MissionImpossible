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

template <typename T, typename... COEFs>
struct AD_Differential_Tuple : public std::tuple<COEFs...>
{
  using base_type = std::tuple<COEFs...>;

 public:
  //  using std::tuple<COEFs...>::tuple;

  AD_Differential_Tuple(const base_type& value) : base_type(value) {}

  const std::tuple<COEFs...>&
  value() const
  {
    return *this;
  };

  friend auto&
  operator<<(std::ostream& out, const AD_Differential_Tuple& to_print)
  {
    std::apply([&out](auto&&... args) { ((out << args << " "), ...); }, to_print.value());
    return out;
  }
};

template <typename T>
auto
create_differential(const AD_Variable_Final_Value_Type_t<T> value,
                    const AD_Variable<AD_Variable<T>>& x)
{
  auto diff_f =
      create_function(value, create_differential(AD_Variable_Final_Value_Type_t<T>(1), x.value()));

  return AD_Differential_Tuple<AD_Variable_Final_Value_Type_t<T>, decltype(diff_f)>(
      std::make_tuple(diff_f));
}

template <typename T, std::size_t N>
struct AD_Differential_Array
{
 protected:
  static_assert(std::is_same_v<T, double>);
  std::array<T, N> _value;
  std::array<std::size_t, N> _index;

 public:
  AD_Differential_Array(){};
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
struct AD_Function<T, AD_Differential_Tuple<T, COEF...>>
{
  static_assert(std::is_same_v<T, double>);  // parano!

  using value_type        = T;
  using differential_type = AD_Differential_Tuple<T, COEF...>;

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
create_function(const T f, const AD_Differential_Tuple<T, COEF...>& df)
{
  return AD_Function<T, AD_Differential_Tuple<T, COEF...>>(f, df);
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

//////////////////////
// Unary chain rule //
//////////////////////
//
template <typename T, std::size_t N>
auto operator*(const AD_Variable_Final_Value_Type_t<T> a, const std::array<T, N>& v)
{
  std::array<T, N> a_v;
  for (size_t i = 0; i < N; ++i)
  {
    a_v[i] = a * v[i];
  }
  return a_v;
}
namespace Detail
{
  template <typename T, typename... COEFs, size_t... Is>
  auto
  multiply_tuple_elements(const T a,
                          const std::tuple<COEFs...>& tuple,
                          const std::index_sequence<Is...>)
  {
    return std::make_tuple(a * std::get<Is>(tuple)...);
  }

}
template <typename T, typename... COEFS>
auto operator*(const AD_Variable_Final_Value_Type_t<T> a,
               const AD_Differential_Tuple<T, COEFS...>& v)
{
  return AD_Differential_Tuple<T, COEFS...>(
      Detail::multiply_tuple_elements(a, v, std::make_index_sequence<sizeof...(COEFS)>()));
}

template <typename T, std::size_t N>
auto
chain_rule(const AD_Variable_Final_Value_Type_t<T> a, const AD_Differential_Array<T, N>& diff)
{
  return AD_Differential_Array<T, N>{a * diff.value(), diff.index()};
}

template <typename T, typename... DIFF>
auto
chain_rule(const AD_Variable_Final_Value_Type_t<T> a, const AD_Differential_Tuple<T, DIFF...>& diff)
{
  return a * diff;
}

template <typename T, typename DIFF>
auto operator*(const AD_Variable_Final_Value_Type_t<T> x0, const AD_Function<T, DIFF>& f1)
{
  return create_function(x0 * f1.value(), chain_rule(x0, f1.df()));
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
  std::cout << y << std::endl;

  auto y2 = 4 * y;
  std::cout << y2 << std::endl;
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

  auto y3 = 4 * y2;
  std::cout << y3 << std::endl;
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
