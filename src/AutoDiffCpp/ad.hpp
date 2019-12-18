#pragma once

#include "AutoDiffCpp/ad_fwd.hpp"
#include "AutoDiffCpp/final_value_type.hpp"
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

   public:  //protected:
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
    const value_type&
    value() const noexcept
    {
      return impl().value();
    }
    decltype(auto)
    index() const noexcept
    {
      return impl().index();
    }
    decltype(auto)
    partialD() const noexcept
    {
      return impl().partialD();
    }

    friend std::ostream&
    operator<<(std::ostream& out, const AD_Crtp<T, IMPL>& to_print)
    {
      out << to_print.value();
      return out;
    }
  };

  ////////
  // AD //
  ////////

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
    //   static thread_local tape_type _tape;
    value_type _value;
    index_array_type _index_array;

   public:
    AD() noexcept {};  // avoid useless default init (zero filled double, int ... for instance)
    AD(const value_type value)
    noexcept  // no explicit (allows AD<float> x=5 syntax
        : _value{value}, _index_array{tape().add_variable()}
    {
    }
    // template <size_t N>
    // AD(const AD_Expr<T, N>& ad)
    // {
    //   (*this) = ad;
    // }

    const value_type&
    value() const noexcept
    {
      return _value;
    };
    const index_array_type&
    index() const noexcept
    {
      return _index_array;
    };

    partialD_array_type
    partialD() const noexcept
    {
      return partialD_array_type{1};
    }

    AD&
    operator=(const T value) noexcept
    {
      _value          = value;
      _index_array[0] = tape().add_variable();
      return *this;
    }

    template <typename IMPL>
    AD&
    operator=(const AD_Crtp<T, IMPL>& ad)
    {
      assert((void*)this != (void*)&ad);

      _value          = ad.value();
      _index_array[0] = tape().row_size();

      tape().add_row(std::integral_constant<std::size_t, AD_Crtp<T, IMPL>::size>(),
                     ad.index().data(),
                     ad.partialD().data());

      return *this;
    }

    // +=, -= etc...
    //
    // a priory cannot be optimized: we use the x+=y -> x=x+y
    // fallback.
    //
    template <typename IMPL>
    auto
    operator+=(const AD_Crtp<T, IMPL>& y)
    {
      *this = *this + y;
      return *this;
    }

    const tape_type&
    tape() const noexcept
    {
      return AutoDiffCpp::tape<T>();
    }

    tape_type&
    tape() noexcept
    {
      return AutoDiffCpp::tape<T>();
    }
  };

  // template <typename T>
  // thread_local typename AD<T>::tape_type AD<T>::_tape = AD<T>::tape_type();

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
    AD_Expr(const value_type value,
            const index_array_type& index_array,
            const partialD_array_type& partialD_array) noexcept
        : _value(value), _index_array(index_array), _partialD_array(partialD_array)
    {
    }

    operator AD<T>() const noexcept
    {
      AD<T> _this;
      _this = *this;
      return _this;
    }

    const value_type&
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

  // Overloading of U*std::array<T>
  // -> used by chain rule
  // Role: provides two specializations,
  // - U is a scalar
  // - U=AD<T> (when std::array<AD<T>>
  //
  // -> IMPORTANT: this avoid artificial creation of AD<T> in nested
  //    case (when std::array<AD<T>>)
  //
  template <typename T, std::size_t N>
  auto operator*(const AD_Final_Value_Type_t<T> scalar_partialD_f, const std::array<T, N>& dg)
  {
    std::array<T, N> to_return;

    for (std::size_t i = 0; i < N; ++i)
    {
      to_return[i] = scalar_partialD_f * dg[i];
    }

    return to_return;
  }


  // template <typename T, typename IMPL, std::size_t N>
  // auto operator*(const AD_Crtp<T, IMPL>& ad_partialD_f, const std::array<AD<T>, N>& dg)
  // {
  //   std::array<AD<T>, N> to_return;

  //   for (std::size_t i = 0; i < N; ++i)
  //   {
  //     to_return[i] = ad_partialD_f * dg[i];
  //   }

  //   return to_return;
  // }

  template <typename T, std::size_t N>
  auto operator*(const AD<T>& ad_partialD_f, const std::array<AD<T>, N>& dg)
  {
    std::array<AD<T>, N> to_return;

    for (std::size_t i = 0; i < N; ++i)
    {
      to_return[i] = ad_partialD_f * dg[i];
    }

    return to_return;
  }

  // f:R->R
  //
  // df○g = ∂0f.dg^0
  //
  // inline auto
  // join(const std::array<T, N0>& v0) noexcept
  // {
  //   return v0;
  // }

  // template <typename T, typename IMPL0>
  // inline AD_Expr<T, IMPL0::size>
  // chain_rule(const Identity_t<T> f_circ_g_value,
  //            const Identity_t<T> partial0,
  //            const AD_Crtp<T, IMPL0>& g0) noexcept
  // {
  //   return {f_circ_g_value, join(g0.index()), partial0 * g0.partialD()};
  // }

  // First order case
  //
  template <typename T, typename IMPL0>
  inline AD_Expr<T, IMPL0::size>
  chain_rule(const AD_Final_Value_Type_t<T> f_circ_g_value,
             const AD_Final_Value_Type_t<T> partial0,
             const AD_Crtp<T, IMPL0>& g0) noexcept
  {
    return {f_circ_g_value, g0.index(), partial0 * g0.partialD()};
  }

  // Nested case specialization to avoid tape-creation of useless
  // AD<T> vartiable storing constants
  //
  // CAVEAT: certainly usefully however I do not understand why it is
  // not used for the moment
  //
  // template <typename T, typename IMPL0, typename IMPL_A, typename IMPL_B>
  // inline AD_Expr<AD<T>, IMPL0::size>
  // chain_rule(const AD_Crtp<T, IMPL_A>& f_circ_g_value,
  //            const AD_Crtp<T, IMPL_B>& partial0,
  //            const AD_Crtp<AD<T>, IMPL0>& g0) noexcept
  // {
  //   const AD<T> ad_f_circ_g_value = f_circ_g_value.impl();
  //   const AD<T> ad_partial0       = partial0.impl();
  //
  //   return {ad_f_circ_g_value, g0.index(), ad_partial0 * g0.partialD()};
  // }

  // Nested case: with scalar partial0
  template <typename T, typename IMPL0, typename IMPL_A>
  inline AD_Expr<AD<T>, IMPL0::size>
  chain_rule(const AD_Crtp<T, IMPL_A>& f_circ_g_value,
             const AD_Final_Value_Type_t<T> partial0,
             const AD_Crtp<AD<T>, IMPL0>& g0) noexcept
  {
    const AD<T> ad_f_circ_g_value = f_circ_g_value.impl();

    return {ad_f_circ_g_value, g0.index(), partial0 * g0.partialD()};
  }

  // f:R2->R
  //
  // df○g = ∂0f.dg^0 + ∂1f.dg^1
  //
  template <typename T, size_t N0, size_t N1>
  inline auto
  join(const std::array<T, N0>& v0, const std::array<T, N1>& v1) noexcept
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
                    const std::array<T, N1>& v1) noexcept
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

  template <typename T, typename IMPL0, typename IMPL1>
  inline AD_Expr<T, IMPL0::size + IMPL1::size>
  chain_rule(const Identity_t<T> f_circ_g_value,
             const Identity_t<T> partial0,
             const Identity_t<T> partial1,
             const AD_Crtp<T, IMPL0>& g0,
             const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return {f_circ_g_value,
            join(g0.index(), g1.index()),
            join_with_product(partial0, partial1, g0.partialD(), g1.partialD())};
  }

  ///////////////
  // operator* //
  ///////////////
  //
  template <typename T, typename IMPL1>
  inline auto operator*(const AD_Final_Value_Type_t<T> g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return chain_rule(g0 * g1.value(), g0, g1);
  }
  template <typename T, typename IMPL0>
  inline auto operator*(const AD_Crtp<T, IMPL0>& g0, const AD_Final_Value_Type_t<T> g1) noexcept
  {
    return chain_rule(g0.value() * g1, g1, g0);
  }
  template <typename T, typename IMPL0, typename IMPL1>
  inline auto operator*(const AD_Crtp<T, IMPL0>& g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return chain_rule(g0.value() * g1.value(), g1.value(), g0.value(), g0, g1);
  }

  ///////////////
  // operator/ //
  ///////////////
  //
  template <typename T, typename IMPL1>
  inline auto
  operator/(const AD_Final_Value_Type_t<T> g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return chain_rule(g0 / g1.value(), -g0 / (g1.value() * g1.value()), g1);
  }
  template <typename T, typename IMPL0>
  inline auto
  operator/(const AD_Crtp<T, IMPL0>& g0, const AD_Final_Value_Type_t<T> g1) noexcept
  {
    return chain_rule(g0.value() / g1, 1 / g1, g0);
  }
  template <typename T, typename IMPL0, typename IMPL1>
  inline auto
  operator/(const AD_Crtp<T, IMPL0>& g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return chain_rule(
        g0.value() / g1.value(), 1 / g1.value(), -g0.value() / (g1.value() * g1.value()), g0, g1);
  }

  ///////////////
  // operator+ //
  ///////////////
  //
  template <typename T, typename IMPL1>
  inline auto
  operator+(const AD_Final_Value_Type_t<T> g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return chain_rule(g0 + g1.value(), +1, g1);
  }
  template <typename T, typename IMPL0>
  inline auto
  operator+(const AD_Crtp<T, IMPL0>& g0, const AD_Final_Value_Type_t<T> g1) noexcept
  {
    return chain_rule(g0.value() + g1, +1, g0);
  }
  template <typename T, typename IMPL0, typename IMPL1>
  inline auto
  operator+(const AD_Crtp<T, IMPL0>& g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return chain_rule(g0.value() + g1.value(), 1, +1, g0, g1);
  }

  //////////////////////
  // unary operator- //
  /////////////////////
  //
  template <typename T, typename IMPL0>
  inline auto
  operator-(const AD_Crtp<T, IMPL0>& g0) noexcept
  {
    return chain_rule(-g0.value(), -1, g0);
  }

  ///////////////
  // operator- //
  ///////////////
  //
  template <typename T, typename IMPL1>
  inline auto
  operator-(const AD_Final_Value_Type_t<T> g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return chain_rule(g0 - g1.value(), -1, g1);
  }
  template <typename T, typename IMPL0>
  inline auto
  operator-(const AD_Crtp<T, IMPL0>& g0, const AD_Final_Value_Type_t<T> g1) noexcept
  {
    return chain_rule(g0.value() - g1, +1, g0);
  }
  template <typename T, typename IMPL0, typename IMPL1>
  inline auto
  operator-(const AD_Crtp<T, IMPL0>& g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return chain_rule(g0.value() - g1.value(), 1, -1, g0, g1);
  }

  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////
  //
  // Comparison operators ==, !=
  //
  // Here this is quite simple as we only return a boolean (no tape manipulation)
  //

  ////////////////
  // operator== //
  ////////////////
  //
  template <typename T, typename IMPL1>
  inline bool
  operator==(const AD_Final_Value_Type_t<T> g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return g0 == g1.value();
  }
  template <typename T, typename IMPL0>
  inline bool
  operator==(const AD_Crtp<T, IMPL0>& g0, const AD_Final_Value_Type_t<T> g1) noexcept
  {
    return g0.value() == g1;
  }

  template <typename T, typename IMPL0, typename IMPL1>
  inline bool
  operator==(const AD_Crtp<T, IMPL0>& g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return g0.value() == g1.value();
  }
}
