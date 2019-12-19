// somes test a new implementation with the aim of being able to have
// delayed tape registration not only for first order but also high
// order derivative

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

  value_type _value;
  index_type _index;

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
};

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
};

// Function
template <typename T, typename AD_DIFFERENTIAL>
struct AD_Function
{
  using value_type       = T;
  using final_value_type = AD_Variable_Final_Value_Type_t<value_type>;

  value_type _f_value;
  // AD_Differential must be AD_Differential_Crtp<T,...>
  AD_DIFFERENTIAL _df_value;
};

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
  differential_type diff{._df_value = {a}, ._df_index = {x.index()}};

  using function_type = AD_Function<T, differential_type>;

  return function_type{._f_value = a * x.value(), ._df_value = diff};
}

void
test_0()
{
  using T = double;

  AD_Variable<T> x0{._value = 2, ._index = 0};

  auto y = 5 * x0;
  (void)y;
}
int
main()
{
  // using T = double;

  // AD_AD<T> d_x0{._value = 2, ._index = 0};
  // AD_AD<AD_AD<T>> x0{._value = d_x0, ._index = 0};

  // auto y = 5 * x0;
}
