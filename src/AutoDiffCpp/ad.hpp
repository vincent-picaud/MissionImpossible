#pragma once

#include "AutoDiffCpp/identity.hpp"
#include "AutoDiffCpp/tape.hpp"
#include "AutoDiffCpp/type_traits.hpp"

#include <array>

namespace AutoDiffCpp
{
  /////////////
  // AD_Crtp //
  /////////////
  //
  template <typename T, typename IMPL>
  class AD_Crtp
  {
   public:
    IMPL&
    impl() noexcept
    {
      return static_cast<IMPL&>(*this);
    };
    const IMPL&
    impl() const noexcept
    {
      return static_cast<IMPL&>(*this);
    };
  };

  ////////
  // AD //
  ////////

  template <typename T>
  class AD;
  template <typename T, size_t N>
  class AD_Expr;

  template <typename T>
  class AD : public AD_Crtp<T, AD<T>>
  {
   public:
    using tape_type  = Tape<T>;
    using index_type = typename tape_type::index_type;
    using value_type = typename tape_type::value_type;

   private:
    static thread_local tape_type _tape;
    value_type _value;
    index_type _index;

   public:
    //    AD(const AD&) = default;
    AD(const value_type& value) noexcept
    {
      _value = value;
      _index = _tape.add_variable();
    }

    value_type
    value() const noexcept
    {
      return _value;
    };
    index_type
    index() const noexcept
    {
      return _index;
    };

    value_type
    partialD() const noexcept
    {
      return 1;
    }
  };

  template <typename T>
  thread_local typename AD<T>::tape_type AD<T>::_tape = AD<T>::tape_type();

  /////////////
  // AD_Expr //
  /////////////

  template <typename T, size_t N>
  class AD_Expr : public AD_Crtp<T, AD_Expr<T, N>>
  {
   public:
    using index_type          = typename AD<T>::index_type;
    using value_type          = typename AD<T>::value_type;
    using index_array_type    = std::array<index_type, N>;
    using partialD_array_type = std::array<value_type, N>;

   protected:
    value_type _value;
    index_array_type _index_array;
    partialD_array_type _partialD_array;

   public:
    //    AD_Expr(const AD_Expr&) = default;

    AD_Expr(const value_type value,
            const index_array_type& index_array,
            const partialD_array_type& partialD_array) noexcept
        : _value(value), _index_array(index_array), _partialD_array(partialD_array)
    {
    }

    value_type
    value() const noexcept
    {
      return _value;
    };

    const index_array_type&
    index() const noexcept
    {
      return _index_array;
    }

    const partialD_array_type&
    partialD() const noexcept
    {
      return _partialD_array;
    }
  };

  // template <typename T, size_t N>
  // inline std::array<T, N> chain_rule_helper(const Identity_t<T> alpha, const std::array<T, N>& vector)
  // {
  //   std::array<T, N> dest;
  //   for (std::size_t i = 0; i < N; ++i)
  //   {
  //     dest[i] = alpha * vector[i];
  //   }
  //   return dest;
  // }
  template <typename T>
  inline auto
  join(const T v0, const T v1)
  {
    return std::array<T, 2>({v0, v1});
  }

  template <typename T>
  inline auto
  join_with_product(const T partialD_1_f, const T partialD_2_f, const T dg1, const T dg2)
  {
    return std::array<T, 2>({partialD_1_f * dg1, partialD_2_f * dg2});
  }

  // f:R2->R
  //
  // df○g = ∂1f.dg^1 + ∂2f.dg^2
  //
  template <typename T>
  inline AD_Expr<T, 2>
  chain_rule(const Identity_t<T> f_circ_g_value,
             const Identity_t<T> partial1,
             const Identity_t<T> partial2,
             const AD<T>& g1,
             const AD<T>& g2)
  {
    return {f_circ_g_value,
            join(g1.index(), g2.index()),
            join_with_product(partial1, partial2, g1.partialD(), g2.partialD())};
  }

  template <typename T>
  AD_Expr<T, 1> operator*(const Identity_t<T> a0, const AD<T>& a1)
  {
    using return_type         = decltype(a0 * a1);
    using index_array_type    = typename return_type::index_array_type;
    using partialD_array_type = typename return_type::partialD_array_type;

    return return_type{a0 * a1.value(), index_array_type({a1.index()}), partialD_array_type({a0})};
  }

  // template <typename T, typename IMPL0, typename IMPL1>
  // AD_Expr<T, 1> operator*(const AD_Crtp<T, IMPL0>& a0, const AD_Crtp<T, IMPL1>& a1)
  // {
  //   return AD_Expr<T, 1>{
  //       a0 * a1.value(), std::array<index_type, 1>({a1.index()}), std::array<T, 1>({a0})};
  // }
}  // namespace AutoDiffCpp
