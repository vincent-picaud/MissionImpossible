// somes test a new implementation with the aim of being able to have
// delayed tape registration not only for first order but also high
// order derivative

#include <tuple>

template <typename T>
struct B_AD
{
  using value_type = T;
  using index_type = std::size_t;

  value_type _value;
  index_type _index;
};

template <typename T, typename IMPL>
struct B_Diff_Crtp
{
};

template <typename T, typename D0, typename... Di>
struct B_Diff_Expr : public B_Diff_Crtp<T, B_Diff_Expr<T, D0, Di...>>
{
  std::tuple<D0, Di...> _diff;
};

template <typename T, size_t N>
struct B_Diff_Terminal : public B_Diff_Crtp<T, B_Diff_Terminal<T, N>>
{
  typename B_AD<T>::value_type _f_value;
  // for efficiency reason we split apart value and index
  // -> this allows to use vectorization for value
  std::array<typename B_AD<T>::value_type, N> _df_value_array; // value_index defining df
  std::array<typename B_AD<T>::index_type, N> _df_index_array;
};

int
main()
{
}
