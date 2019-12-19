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

// Used for expression template/lazy
//
template <typename T, typename IMPL>
struct AD_Differential_Crtp
{
};

template <typename T, typename COEF_0, typename... COEF_i>
struct AD_Differential : public AD_Differential_Crtp<T, AD_Differential<T, COEF_0, COEF_i...>>
{
  std::tuple<COEF_0, COEF_i...> _df;  // value_index defining df
};

template <typename T, size_t N>
struct AD_Differential_Terminal;

// df coefficient are AD<T> var, f value is T
template <typename T, size_t N>
struct AD_Differential_Terminal
{
  using value_type = T;
  using index_type = std::size_t;

  // for efficiency reason we split apart value and index
  // -> this allows to use vectorization for value
  std::array<value_type, N> _df_value;
  std::array<index_type, N> _df_index;

  friend std::ostream&
  operator<<(std::ostream& out, const AD_Differential_Terminal& to_print)
  {
    out << " differential: ";
    for (std::size_t i = 0; i < N; i++)
    {
      out << "d_" << to_print._df_index[i] << "=" << to_print._df_value[i] << ", ";
    }
    out << std::endl;

    return out;
  }
};

// Differential operations only + and scalar * <- enough to define chain rule

// Function
template <typename T, typename AD_DIFFERENTIAL>
struct AD_Function
{
  static_assert(std::is_same_v<T, double>);

  using value_type       = T;
  using final_value_type = AD_Variable_Final_Value_Type_t<value_type>;

  value_type _f_value;
  // AD_Differential must be AD_Differential_Crtp<T,...>
  AD_DIFFERENTIAL _df_value;

  friend std::ostream&
  operator<<(std::ostream& out, const AD_Function& to_print)
  {
    out << " f_value: " << to_print._f_value << std::endl;
    out << "df_value: " << to_print._df_value << std::endl;
    return out;
  }
};

template <typename T, typename AD_DIFFERENTIAL>
AD_Function<T, AD_DIFFERENTIAL>
create_ad_function(const T& value, const AD_DIFFERENTIAL& differential)
{
  return {value, differential};
}

// f: R^n->R
// template <typename T, typename IMPL_F, typename... IMPL_Gi>
// auto
// chain_rule(const AD_Differential_Crtp<AD_AD<T>, IMPL_F>& f,
//            const AD_Differential_Crtp<AD_AD<T>, IMPL_Gi...>& gi)
// {
// }

// Init computations: a scalar and AD
// -> must also include two AD
template <typename T>
auto operator*(const AD_Variable_Final_Value_Type_t<T>& a, const AD_Variable<T>& x)
{
  using differential_type = AD_Differential_Terminal<T, 1>;
  differential_type diff{{a}, {x.index()}};

  return create_ad_function(a * x.final_value(), diff);
}

template <typename T>
auto operator*(const AD_Variable<T>& x0, const AD_Variable<T>& x1)
{
  using differential_type = AD_Differential_Terminal<T, 2>;
  differential_type diff{{x1.value(), x0.value()}, {x0.index(), x1.index()}};

  return create_ad_function(x0.final_value() * x1.final_value(), diff);
}

template <typename F, typename DIFF>
auto operator*(const AD_Function<F, DIFF>& f0, const AD_Variable<typename F::value_type>& x1)
{
  auto diff = f0.df() * AD_Differential_Terminal{{1}, {x1.index()}};

  return create_ad_function(f0.f() * x1.value(), diff);
}
// template <typename T, typename DIFF>
// auto operator*(const AD_Variable_Final_Value_Type_t<T>& a, const AD_Function<T, DIFF>& x)
// {
//   // using differential_type = AD_Differential_Terminal<T, 1>;
//   // differential_type diff{._df_value = {a}, ._df_index = {x.index()}};

//   // using function_type = AD_Function<T, differential_type>;

//   // return function_type{._f_value = a * x.value(), ._df_value = diff};
//   return 0;
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

  auto y = 5 * x0;
  std::cout << y << std::endl;
}

void
test_1()
{
  reset_global_index();
  std::cout << "===> " << __PRETTY_FUNCTION__ << std::endl;

  using T = double;

  AD_Variable<T> x0{2};
  AD_Variable<T> x1{3};

  auto y = x0 * x1;
  std::cout << y << std::endl;
}
int
main()
{
  test_0();
  test_1();

  reset_global_index();
  std::cout << "===> " << __PRETTY_FUNCTION__ << std::endl;

  using T = double;

  AD_Variable<AD_Variable<T>> x0{2};
  // AD_AD<T> d_x0{._value = 2, ._index = 0};
  // AD_AD<AD_AD<T>> x0{._value = d_x0, ._index = 0};

  auto y = 5 * x0;
  // auto y2 = y * x0;
  // printType(y);

  std::cout << y;
}
