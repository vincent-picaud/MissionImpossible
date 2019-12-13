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
    using tape_type  = Tape<T>;
    using index_type = typename tape_type::index_type;
    using value_type = typename tape_type::value_type;

    using type_traits          = Type_Traits<IMPL>;
    static constexpr auto size = type_traits::size;
    using index_array_type     = std::array<index_type, size>;
    using partialD_array_type  = std::array<value_type, size>;

   protected:
    IMPL&
    impl() noexcept
    {
      return static_cast<IMPL&>(*this);
    };
    const IMPL&
    impl() const noexcept
    {
      return static_cast<const IMPL&>(*this);
    };

   public:
    value_type
    value() const
    {
      return impl().value();
    }
    const index_array_type&
    index() const
    {
      return impl().index();
    }
    const partialD_array_type&
    partialD() const
    {
      return impl().partialD();
    }
  };

  ////////
  // AD //
  ////////

  template <typename T>
  class AD;
  template <typename T, size_t N>
  class AD_Expr;

  template <typename T>
  struct Type_Traits<AD<T>>
  {
    static constexpr std::size_t size = 1;
  };

  template <typename T>
  class AD : public AD_Crtp<T, AD<T>>
  {
   public:
    using base_type           = AD_Crtp<T, AD<T>>;
    using tape_type           = typename base_type::tape_type;
    using index_type          = typename base_type::index_type;
    using value_type          = typename base_type::value_type;
    using index_array_type    = typename base_type::index_array_type;
    using partialD_array_type = typename base_type::partialD_array_type;

   private:
    static thread_local tape_type _tape;
    value_type _value;
    index_array_type _index_array;
    static constexpr partialD_array_type _partialD_array{1};

   public:
    //    AD(const AD&) = default;
    AD(const value_type value) noexcept : _value{value}, _index_array{_tape.add_variable()} {}

    value_type
    value() const noexcept
    {
      return _value;
    };
    const index_array_type&
    index() const noexcept
    {
      return _index_array;
    };

    const partialD_array_type&
    partialD() const noexcept
    {
      return _partialD_array;
    }
  };

  template <typename T>
  thread_local typename AD<T>::tape_type AD<T>::_tape = AD<T>::tape_type();

  /////////////
  // AD_Expr //
  /////////////

  template <typename T, size_t N>
  struct Type_Traits<AD_Expr<T, N>>
  {
    static constexpr std::size_t size = N;
  };

  template <typename T, size_t N>
  class AD_Expr : public AD_Crtp<T, AD_Expr<T, N>>
  {
   public:
    using base_type           = AD_Crtp<T, AD_Expr<T, N>>;
    using index_type          = typename base_type::index_type;
    using value_type          = typename base_type::value_type;
    using index_array_type    = typename base_type::index_array_type;
    using partialD_array_type = typename base_type::partialD_array_type;

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
  // template <typename T>
  // inline auto
  // join(const T v0, const T v1)
  // {
  //   return std::array<T, 2>({v0, v1});
  // }

  template <typename T, size_t N0, size_t N1>
  inline auto
  join(const std::array<T, N0>& v0, const std::array<T, N1>& v1)
  {
    std::array<T, N0 + N1> to_return;

    for (std::size_t i = 0; i < N0; ++i)
    {
      to_return[i] = v0[i];
    }
    for (std::size_t i = 0; i < N1; ++i)
    {
      to_return[N0 + i] = v1[i];
    }

    return to_return;
  }
  template <typename T, size_t N0, size_t N1>
  inline auto
  join_with_product(const Identity_t<T> partialD_0_f,
                    const Identity_t<T> partialD_1_f,
                    const std::array<T, N0>& v0,
                    const std::array<T, N1>& v1)
  {
    std::array<T, N0 + N1> to_return;

    for (std::size_t i = 0; i < N0; ++i)
    {
      to_return[i] = partialD_0_f * v0[i];
    }
    for (std::size_t i = 0; i < N1; ++i)
    {
      to_return[N0 + i] = partialD_1_f * v1[i];
    }

    return to_return;
  }

  // f:R2->R
  //
  // df○g = ∂0f.dg^0 + ∂2f.dg^2
  //
  template <typename T, typename IMPL0, typename IMPL1>
  inline AD_Expr<T, IMPL0::size + IMPL1::size>
  chain_rule(const Identity_t<T> f_circ_g_value,
             const Identity_t<T> partial0,
             const Identity_t<T> partial1,
             const AD_Crtp<T, IMPL0>& g0,
             const AD_Crtp<T, IMPL1>& g1)
  {
    return {f_circ_g_value,
            join(g0.index(), g1.index()),
            join_with_product(partial0, partial1, g0.partialD(), g1.partialD())};
  }

  template <typename T>
  AD_Expr<T, 1> operator*(const Identity_t<T> g0, const AD<T>& g1)
  {
    using return_type         = decltype(g0 * g1);
    using index_array_type    = typename return_type::index_array_type;
    using partialD_array_type = typename return_type::partialD_array_type;

    return return_type{g0 * g1.value(), index_array_type({g1.index()}), partialD_array_type({g0})};
  }

  template <typename T, typename IMPL0, typename IMPL1>
  auto operator*(const AD_Crtp<T, IMPL0>& g0, const AD_Crtp<T, IMPL1>& g1)
  {
    return chain_rule(g0.value() * g1.value(), g1.value(), g0.value(), g0, g1);
  }
}  // namespace AutoDiffCpp
