
#include <array>
#include <iostream>
#include <tuple>

//////////////////////////////////////////////////////////////////
// Some meta programming tools
//////////////////////////////////////////////////////////////////

template <class T>
struct type_identity
{
  using type = T;
};
template <class T>
using type_identity_t = typename type_identity<T>::type;

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

template <typename... COEFs>
struct AD_Differential_Tuple;

template <typename T, std::size_t N>
struct AD_Differential_Array;

//================

template <typename T>
struct Is_AD_Function : std::false_type
{
};

template <typename T, std::size_t N>
struct Is_AD_Function<AD_Function<T, AD_Differential_Array<T, N>>> : std::true_type
{
};
template <typename T, typename... COEFF>
struct Is_AD_Function<AD_Function<T, AD_Differential_Tuple<COEFF...>>> : std::true_type
{
};
template <typename T>
inline constexpr bool Is_AD_Function_v = Is_AD_Function<T>::value;

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
  // Commented how because test_0 fails here (and this would be normal)
  //
  // static_assert((Is_AD_Function_v<COEFs> && ...));

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

  const value_array_type&
  value() const
  {
    return _value_array;
  };
  const index_array_type&
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
// Define Function with its gradient
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//
// Specialization Terminal Node:
//
// -> this is when differential coefficients are scalar constants
//
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
    out << "{ f=" << to_print._value << "\tdf=[ ";
    for (size_t i = 0; i < N; ++i)
    {
      out << "d" << to_print._df.index()[i] << "=" << to_print._df.value()[i] << ", ";
    }
    out << "] }";
    return out;
  }
};

template <typename T, size_t N>
auto
create_function(const T f, const AD_Differential_Array<T, N>& df)
{
  return AD_Function<T, AD_Differential_Array<T, N>>(f, df);
}

// Create a function from an AD_Variable
// Note: this function must only catch AD_Variable<T> with T = double, float,
//
// Test:
// - test_2()
//
template <typename T,
          typename = std::enable_if_t<std::is_same_v<T, AD_Variable_Final_Value_Type_t<T>>>>
auto
create_partial_derivative(const AD_Variable<T>& x)
{
  // not x is only use to get the index
  return AD_Differential_Array<T, 1>{{T(1)}, {x.index()}};
}

template <typename T,
          typename = std::enable_if_t<std::is_same_v<T, AD_Variable_Final_Value_Type_t<T>>>>
auto
create_function(const AD_Variable<T>& x)
{
  return create_function(x.final_value(), create_partial_derivative(x));
}

//////////////////////////////////////////////////////////////////
//
// Specialization when differential coefficients are them self functions
//
template <typename T, typename... COEF>
struct AD_Function<T, AD_Differential_Tuple<COEF...>>
{
  static_assert(std::is_same_v<T, double>);  // parano!
  static_assert((Is_AD_Function_v<COEF> && ...));

  using value_type        = T;
  using differential_type = AD_Differential_Tuple<COEF...>;

 protected:
  value_type _f_value;
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
    out << "{ f=" << to_print.value() << "\tdf=[ " << to_print.df() << "] }";

    return out;
  }
};

template <typename T, typename... COEF>
auto
create_function(const T f, const AD_Differential_Tuple<COEF...>& df)
{
  static_assert(std::is_same_v<T, double>);  // parano!
  return AD_Function<T, AD_Differential_Tuple<COEF...>>(f, df);
}

// Create a function from an high order AD_Variable
//
// Test:
// - test_3()
//
// template <typename T>
// auto
// create_null_partial(const AD_Variable<AD_Variable<T>>& x)
// {
// return create_function(AD_Variable_Final_Value_Type_t<T>(0)x.final_value(),
// }
template <typename T>
auto
create_partial_derivative(const AD_Variable<AD_Variable<T>>& x)
{
  auto df_func =
      create_function(AD_Variable_Final_Value_Type_t<T>(0), create_partial_derivative(x.value()));
  auto df = AD_Differential_Tuple<decltype(df_func)>{std::make_tuple(df_func), {x.index()}};

  return df;
}
template <typename T>
auto
create_function(const AD_Variable<AD_Variable<T>>& x)
{
  return create_function(x.final_value(), create_partial_derivative(x));
}

//////////////////////////////////////////////////////////////////

// Array chain rule
template <typename T, size_t N>
std::array<T, N> operator*(const T a, const std::array<T, N>& v)
{
  std::array<T, N> a_v;
  for (size_t i = 0; i < N; ++i)
  {
    a_v[i] = a * v[i];
  }
  return a_v;
}

template <typename T, size_t N>
auto
chain_rule(const T a, const AD_Differential_Array<T, N>& df)
{
  return AD_Differential_Array<T, N>(a * df.value(), df.index());
}

// Tuple chain rule
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
auto operator*(const T a, const std::tuple<COEFS...>& v)
{
  return Detail::multiply_tuple_elements(a, v, std::make_index_sequence<sizeof...(COEFS)>());
}

template <typename T, typename... COEFS>
auto
chain_rule(const T a, const AD_Differential_Tuple<COEFS...>& df)
{
  return AD_Differential_Tuple<COEFS...>(a * df.value(), df.index());
}

template <typename T, typename D1>
auto operator*(const AD_Variable_Final_Value_Type_t<T> a, const AD_Function<T, D1>& f1)
{
  return create_function(a * f1.value(), chain_rule(a, f1.df()));
}

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

void
test_2()
{
  reset_global_index();
  std::cout << "\n\n===> " << __PRETTY_FUNCTION__ << std::endl;

  AD_Variable<double> x(3);
  auto f = create_function(x);

  std::cout << f;
}

void
test_3()
{
  reset_global_index();
  std::cout << "\n\n===> " << __PRETTY_FUNCTION__ << std::endl;

  AD_Variable<double>::global_index = 100;

  AD_Variable<AD_Variable<AD_Variable<double>>> x(3);

  std::cout << x << std::endl;

  auto f = create_function(x);

  std::cout << f;
}

void
test_4()
{
  reset_global_index();
  std::cout << "\n\n===> " << __PRETTY_FUNCTION__ << std::endl;

  AD_Variable<double>::global_index = 100;

  AD_Variable<double> x1(3);

  auto y = 2 * create_function(x1);

  std::cout << y;
}
void
test_4b()
{
  reset_global_index();
  std::cout << "\n\n===> " << __PRETTY_FUNCTION__ << std::endl;

  AD_Variable<double>::global_index = 100;

  AD_Variable<AD_Variable<AD_Variable<double>>> x1(3);

  auto y = 2 * create_function(x1);

  std::cout << y;
}
int
main()
{
  test_0();
  test_1();
  test_2();
  test_3();
  test_4();
  test_4b();
}
