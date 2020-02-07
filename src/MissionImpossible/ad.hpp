#pragma once

#include "MissionImpossible/ad_fwd.hpp"
#include "MissionImpossible/identity.hpp"
#include "MissionImpossible/tape.hpp"
#include "MissionImpossible/underlying_type.hpp"

#include <array>
#include <tuple>

namespace MissionImpossible
{
  /////////////
  // AD_Crtp //
  /////////////

  template <typename T>
  struct AD_Types
  {
    using tape_type  = Tape<T>;
    using index_type = typename tape_type::index_type;
    using value_type = typename tape_type::value_type;
  };

#define AD_TYPES(T)                                  \
  using types_type = AD_Types<T>;                    \
  using tape_type  = typename types_type::tape_type; \
  using index_type = typename tape_type::index_type; \
  using value_type = typename tape_type::value_type;

  template <typename T, typename IMPL>
  class AD_Crtp : AD_Types<T>
  {
   public:
    AD_TYPES(T);

    IMPL&
    impl() noexcept
    {
      return static_cast<IMPL&>(*this);
    }
    const IMPL&
    impl() const noexcept
    {
      return static_cast<const IMPL&>(*this);
    }

    // CRTP Static interface
    //================================================================
    //

    // value()
    //----------------------------------------------------------------
    // CAVEAT:
    //
    // #+begin_src cpp
    // AD<AD<double>> y;
    //
    // // what value() means:
    // AD<double> y_value = y.value();
    //
    // // what it does not mean (compilation error):
    // //
    // // double y_value = y.value();
    // //
    // #+end_src
    //
    decltype(auto)
    value() const noexcept
    {
      return impl().value();
    }
    decltype(auto)
    differential() const noexcept
    {
      return impl().differential();
    }
    const tape_type&
    tape() const noexcept
    {
      return MissionImpossible::tape<T>();
    }
    tape_type&
    tape() noexcept
    {
      return MissionImpossible::tape<T>();
    }
  };

  //////////////////////////////////
  // AD_Differential & chain rule //
  //////////////////////////////////

  template <typename T, std::size_t N>
  class AD_Differential : public AD_Types<T>
  {
   public:
    AD_TYPES(T);

    using value_array_type = std::array<T, N>;
    using index_array_type = std::array<index_type, N>;

   protected:
    value_array_type _value_array;
    index_array_type _index_array;
#ifndef NDEBUG
    static constexpr index_type _bad_index = -1;
#endif
    
   public:
    AD_Differential() noexcept
    {
#ifndef NDEBUG
      if constexpr (N > 0) _index_array[0] = _bad_index;
#endif
    }
    AD_Differential(const value_array_type& value_array,
                    const index_array_type& index_array) noexcept
        : _value_array(value_array), _index_array(index_array)
    {
    }

    const value_array_type&
    value() const noexcept
    {
      assert((N == 0) || (_index_array[0] != _bad_index));
      return _value_array;
    };
    const index_array_type&
    index() const noexcept
    {
      assert((N == 0) || (_index_array[0] != _bad_index));
      return _index_array;
    }
  };

  //  To implement chain rule
  //
  //  df○g = ∂0f.dg^0 +  ∂1f.dg^1 + ...
  //
  // It is enough to define:
  // a. differential + differential
  // b. scalar * differential
  //
  namespace Detail
  {
    template <typename U, typename T, std::size_t N>
    inline auto operator*(const U& u, const std::array<T, N>& a) noexcept
    {
      std::array<T, N> to_return;

      for (std::size_t i = 0; i < N; ++i)
      {
        to_return[i] = u * a[i];
      }

      return to_return;
    }

    template <typename T, std::size_t N>
    inline auto
    operator-(const std::array<T, N>& a) noexcept
    {
      std::array<T, N> to_return;

      for (std::size_t i = 0; i < N; ++i)
      {
        to_return[i] = -a[i];
      }

      return to_return;
    }

    //================================================================

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

  }  // Detail

  template <typename T, typename IMPL, std::size_t N>
  inline auto operator*(const AD_Crtp<T, IMPL>& v, const AD_Differential<T, N>& dg0) noexcept
  {
    using Detail::operator*;

    return AD_Differential{v * dg0.value(), dg0.index()};
  }

  // Explanation: used by op* for instance:
  //
  // return AD_Function{g0.value() * g1.value(),
  //                   g1.value() * g0.differential() + g0.value() * g1.differential()};
  //
  template <typename T, typename IMPL, std::size_t N>
  inline auto operator*(const AD_Crtp<T, IMPL>& v, const AD_Differential<AD<T>, N>& dg0) noexcept
  {
    using Detail::operator*;

    return AD_Differential{v * dg0.value(), dg0.index()};
  }

  template <typename T, std::size_t N>
  inline auto operator*(const AD_Underlying_Type_t<T> v, const AD_Differential<T, N>& dg0) noexcept
  {
    using namespace Detail;

    return AD_Differential{v * dg0.value(), dg0.index()};
  }
  template <typename T, std::size_t N>
  inline auto
  operator-(const AD_Differential<T, N>& dg0) noexcept
  {
    using Detail::operator-;

    return AD_Differential{-dg0.value(), dg0.index()};
  }

  template <typename T, std::size_t N0, std::size_t N1>
  inline auto
  operator+(const AD_Differential<T, N0>& dg0, const AD_Differential<T, N1>& dg1) noexcept
  {
    using Detail::join;

    return AD_Differential{join(dg0.value(), dg1.value()), join(dg0.index(), dg1.index())};
  }

  //////////////////////////
  // AD (active variable) //
  //////////////////////////

  template <typename T>
  class AD : public AD_Crtp<T, AD<T>>
  {
   public:
    AD_TYPES(T);

    using differential_type = AD_Differential<T, 1>;

   protected:
    value_type _value;
    differential_type _dvalue;

   public:
    AD() noexcept {};  // avoid useless default init (zero filled double, int ... for instance)
    AD(const AD_Underlying_Type_t<value_type> value) noexcept : AD() { (*this) = value; }
    template <typename VALUE_TYPE, std::size_t N>
    AD(const AD_Function<T, VALUE_TYPE, N>& ad)
    {
      (*this) = ad;
    }

    AD&
    operator=(const AD_Underlying_Type_t<value_type> value) noexcept
    {
      _value  = value;
      _dvalue = differential_type{{value_type(1)}, {this->tape().add_variable()}};
      return *this;
    }

    template <typename VALUE_TYPE, std::size_t N>
    AD&
    operator=(const AD_Function<T, VALUE_TYPE, N>& ad)
    {
      assert((void*)this != (void*)&ad);

      _value  = ad.value();
      _dvalue = differential_type{{value_type(1)}, {this->tape().row_size()}};

      //      TODO rename: add_row > add_differential
      this->tape().add_row(std::integral_constant<std::size_t, N>(),
                           ad.differential().index().data(),
                           ad.differential().value().data());

      return *this;
    }

    // +=, -= etc...
    //
    // a priory cannot be optimized:
    // we use the x+=y -> x=x+y
    // fallback.
    //
    template <typename IMPL>
    auto
    operator+=(const AD_Crtp<T, IMPL>& y)
    {
      *this = *this + y;
      return *this;
    }
    template <typename IMPL>
    auto
    operator-=(const AD_Crtp<T, IMPL>& y)
    {
      *this = *this - y;
      return *this;
    }
    template <typename IMPL>
    auto
    operator*=(const AD_Crtp<T, IMPL>& y)
    {
      *this = *this * y;
      return *this;
    }
    template <typename IMPL>
    auto
    operator/=(const AD_Crtp<T, IMPL>& y)
    {
      *this = *this / y;
      return *this;
    }

    const index_type&
    index() const noexcept
    {
      return _dvalue.index()[0];
    }

    const value_type&
    value() const noexcept
    {
      return _value;
    }
    const differential_type&
    differential() const noexcept
    {
      return _dvalue;
    }
    friend std::ostream&
    operator<<(std::ostream& out, const AD& to_print)
    {
      out << std::showpos << to_print.value();
      return out;
    }
  };

  ///////////////////////////////////////////////////////////////
  // AD_Function, static array to store value and differential //
  ///////////////////////////////////////////////////////////////

  template <typename T, typename VALUE_TYPE, size_t N>
  class AD_Function : public AD_Crtp<T, AD_Function<T, VALUE_TYPE, N>>
  {
   public:
    AD_TYPES(T);

    using differential_type = AD_Differential<T, N>;

   protected:
    VALUE_TYPE _f_value;
    differential_type _df_value;

   public:
    AD_Function(){};
    AD_Function(const VALUE_TYPE& f, const differential_type& df) : _f_value(f), _df_value(df) {}

    // A priori do not define such conversion operator as it opens the
    // door to multiple creations of the same AD<T> var that fills the
    // tape with redundant rows.
    //
    // operator AD<T>() const
    // {
    //   AD<T> y;
    //   y = *this;
    //   return y;
    // }

    const VALUE_TYPE
    value() const
    {
      return _f_value;
    }
    const differential_type&
    differential() const noexcept
    {
      return _df_value;
    }

    friend std::ostream&
    operator<<(std::ostream& out, const AD_Function& to_print)
    {
      out << to_print._f_value;
      return out;
    }
  };

  //////////////////////////
  // Arithmetic operators //
  //////////////////////////

  // operator*
  //////////////////////////////////////////////////////////////////
  //
  template <typename T, typename IMPL>
  inline auto operator*(const AD_Underlying_Type_t<T> g0, const AD_Crtp<T, IMPL>& g1) noexcept
  {
    return AD_Function{g0 * g1.value(), g0 * g1.differential()};
  }
  template <typename T, typename IMPL>
  inline auto operator*(const AD_Crtp<T, IMPL>& g1, const AD_Underlying_Type_t<T> g0) noexcept
  {
    return g0 * g1;
  }

  template <typename T, typename IMPL0, typename IMPL1>
  inline auto operator*(const AD_Crtp<T, IMPL0>& g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return AD_Function{g0.value() * g1.value(),
                       g1.value() * g0.differential() + g0.value() * g1.differential()};
  }

  // operator/
  //////////////////////////////////////////////////////////////////
  //
  template <typename T, typename IMPL1>
  inline auto
  operator/(const AD_Underlying_Type_t<T> g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return AD_Function(g0 / g1.value(), (-g0 / (g1.value() * g1.value())) * g1.differential());
  }
  template <typename T, typename IMPL0>
  inline auto
  operator/(const AD_Crtp<T, IMPL0>& g0, const AD_Underlying_Type_t<T> g1) noexcept
  {
    return AD_Function(g0.value() / g1, (1 / g1) * g0.differential());
  }
  template <typename T, typename IMPL0, typename IMPL1>
  inline auto
  operator/(const AD_Crtp<T, IMPL0>& g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return AD_Function(g0.value() / g1.value(),
                       (1 / g1.value()) * g0.differential() +
                           (-g0.value() / (g1.value() * g1.value())) * g1.differential());
  }

  // operator+
  //////////////////////////////////////////////////////////////////
  //
  template <typename T, typename IMPL1>
  inline auto
  operator+(const AD_Underlying_Type_t<T> g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return AD_Function(g0 + g1.value(), g1.differential());
  }
  template <typename T, typename IMPL0>
  inline auto
  operator+(const AD_Crtp<T, IMPL0>& g0, const AD_Underlying_Type_t<T> g1) noexcept
  {
    return AD_Function(g0.value() + g1, g0.differential());
  }
  template <typename T, typename IMPL0, typename IMPL1>
  inline auto
  operator+(const AD_Crtp<T, IMPL0>& g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return AD_Function(g0.value() + g1.value(), g0.differential() + g1.differential());
  }

  // unary operator-
  //////////////////////////////////////////////////////////////////
  //
  template <typename T, typename IMPL0>
  inline auto
  operator-(const AD_Crtp<T, IMPL0>& g0) noexcept
  {
    return AD_Function(-g0.value(), -g0.differential());
  }

  // operator-
  //////////////////////////////////////////////////////////////////
  //
  template <typename T, typename IMPL1>
  inline auto
  operator-(const AD_Underlying_Type_t<T> g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return AD_Function(g0 - g1.value(), -g1.differential());
  }
  template <typename T, typename IMPL0>
  inline auto
  operator-(const AD_Crtp<T, IMPL0>& g0, const AD_Underlying_Type_t<T> g1) noexcept
  {
    return AD_Function(g0.value() - g1, g0.differential());
  }
  template <typename T, typename IMPL0, typename IMPL1>
  inline auto
  operator-(const AD_Crtp<T, IMPL0>& g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return AD_Function(g0.value() - g1.value(), g0.differential() + (-g1.differential()));
  }

  /////////////////////////////////
  // Comparison operators ==, != //
  /////////////////////////////////
  //
  // Here this is quite simple as we only return a boolean (no tape manipulation)
  //

  // operator==
  //
  template <typename T, typename IMPL1>
  inline bool
  operator==(const AD_Underlying_Type_t<T> g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return g0 == g1.value();
  }
  template <typename T, typename IMPL0>
  inline bool
  operator==(const AD_Crtp<T, IMPL0>& g0, const AD_Underlying_Type_t<T> g1) noexcept
  {
    return g0.value() == g1;
  }

  template <typename T, typename IMPL0, typename IMPL1>
  inline bool
  operator==(const AD_Crtp<T, IMPL0>& g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return g0.value() == g1.value();
  }

  // operator!=
  //
  template <typename T, typename IMPL1>
  inline bool
  operator!=(const AD_Underlying_Type_t<T> g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return g0 != g1.value();
  }
  template <typename T, typename IMPL0>
  inline bool
  operator!=(const AD_Crtp<T, IMPL0>& g0, const AD_Underlying_Type_t<T> g1) noexcept
  {
    return g0.value() != g1;
  }

  template <typename T, typename IMPL0, typename IMPL1>
  inline bool
  operator!=(const AD_Crtp<T, IMPL0>& g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return g0.value() != g1.value();
  }

  // operator<
  //
  template <typename T, typename IMPL1>
  inline bool
  operator<(const AD_Underlying_Type_t<T> g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return g0 < g1.value();
  }
  template <typename T, typename IMPL0>
  inline bool
  operator<(const AD_Crtp<T, IMPL0>& g0, const AD_Underlying_Type_t<T> g1) noexcept
  {
    return g0.value() < g1;
  }

  template <typename T, typename IMPL0, typename IMPL1>
  inline bool
  operator<(const AD_Crtp<T, IMPL0>& g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return g0.value() < g1.value();
  }

  // operator<=
  //
  template <typename T, typename IMPL1>
  inline bool
  operator<=(const AD_Underlying_Type_t<T> g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return g0 <= g1.value();
  }
  template <typename T, typename IMPL0>
  inline bool
  operator<=(const AD_Crtp<T, IMPL0>& g0, const AD_Underlying_Type_t<T> g1) noexcept
  {
    return g0.value() <= g1;
  }

  template <typename T, typename IMPL0, typename IMPL1>
  inline bool
  operator<=(const AD_Crtp<T, IMPL0>& g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return g0.value() <= g1.value();
  }

  // operator>
  //
  template <typename T, typename IMPL1>
  inline bool
  operator>(const AD_Underlying_Type_t<T> g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return g0 > g1.value();
  }
  template <typename T, typename IMPL0>
  inline bool
  operator>(const AD_Crtp<T, IMPL0>& g0, const AD_Underlying_Type_t<T> g1) noexcept
  {
    return g0.value() > g1;
  }

  template <typename T, typename IMPL0, typename IMPL1>
  inline bool
  operator>(const AD_Crtp<T, IMPL0>& g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return g0.value() > g1.value();
  }

  // operator>=
  //
  template <typename T, typename IMPL1>
  inline bool
  operator>=(const AD_Underlying_Type_t<T> g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return g0 >= g1.value();
  }
  template <typename T, typename IMPL0>
  inline bool
  operator>=(const AD_Crtp<T, IMPL0>& g0, const AD_Underlying_Type_t<T> g1) noexcept
  {
    return g0.value() >= g1;
  }

  template <typename T, typename IMPL0, typename IMPL1>
  inline bool
  operator>=(const AD_Crtp<T, IMPL0>& g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return g0.value() >= g1.value();
  }
}
