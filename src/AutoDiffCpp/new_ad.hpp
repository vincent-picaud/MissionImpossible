#pragma once

//#include "AutoDiffCpp/ad_fwd.hpp"
#include "AutoDiffCpp/final_value_type.hpp"
#include "AutoDiffCpp/identity.hpp"
#include "AutoDiffCpp/tape.hpp"
//#include "AutoDiffCpp/type_traits.hpp"

#include <array>
#include <tuple>

namespace AutoDiffCpp
{
  //////////////////////////////////////////////////////////////////

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

    // Static interface
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
      return AutoDiffCpp::tape<T>();
    }
    tape_type&
    tape() noexcept
    {
      return AutoDiffCpp::tape<T>();
    }
  };

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

   public:
    AD_Differential() {}
    AD_Differential(const value_array_type& value_array, const index_array_type& index_array)
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
    inline auto operator*(const U& u, const std::array<T, N>& a)
    {
      std::array<decltype(u * a[0]), N> to_return;

      for (std::size_t i = 0; i < N; ++i)
      {
        to_return[i] = u * a[i];
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

  }

  // fwd declaration
  template <typename T>
  class AD;

  template <typename T, std::size_t N>
  inline auto operator*(const AD<T>& v, const AD_Differential<T, N>& dg0) noexcept
  {
    using Detail::operator*;

    return AD_Differential{v * dg0.value(), dg0.index()};
  }
  template <typename T, std::size_t N>
  inline auto operator*(const AD_Final_Value_Type_t<T> v, const AD_Differential<T, N>& dg0) noexcept
  {
    using namespace Detail;

    return AD_Differential{v * dg0.value(), dg0.index()};
  }

  template <typename T, std::size_t N0, std::size_t N1>
  inline auto
  operator+(const AD_Differential<T, N0>& dg0, const AD_Differential<T, N1>& dg1) noexcept
  {
    using Detail::join;

    return AD_Differential{join(dg0.value(), dg1.value()), join(dg0.index(), dg1.index())};
  }

  //////////////////////////////////////////////////////////////////

  template <typename T, size_t N>
  class AD_Function;

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
    AD(const AD_Final_Value_Type_t<value_type> value) noexcept : AD() { (*this) = value; }

    AD&
    operator=(const AD_Final_Value_Type_t<value_type> value) noexcept
    {
      _value                 = value;
      const index_type index = this->tape().add_variable();
      _dvalue                = differential_type{{value_type(1)}, {index}};
      return *this;
    }

    operator AD_Function<T, 1>() const { return {_value, _dvalue}; };

    auto
    to_function() const
    {
      return static_cast<AD_Function<T, 1>>(*this);
    };

    // Remplace AD_Expr by function
    // template <std::size_t N>
    //  AD&
    // //    operator=(const AD_Expr<T, N>& ad)
    //  {
    //    assert((void*)this != (void*)&ad);

    //    _value          = ad.value();
    //    _index_array[0] = tape().row_size();

    //    tape().add_row(
    //        std::integral_constant<std::size_t, N>(), ad.index().data(), ad.partialD().data());

    //    return *this;
    //  }

    //     // +=, -= etc...
    // //
    // // a priory cannot be optimized: we use the x+=y -> x=x+y
    // // fallback.
    // //
    // template <typename IMPL>
    // auto
    // operator+=(const AD_Crtp<T, IMPL>& y)
    // {
    //   *this = *this + y;
    //   return *this;
    // }
    // template <typename IMPL>
    // auto
    // operator-=(const AD_Crtp<T, IMPL>& y)
    // {
    //   *this = *this - y;
    //   return *this;
    // }

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
    const index_type&
    index() const noexcept
    {
      return _dvalue.index()[0];
    }

    friend std::ostream&
    operator<<(std::ostream& out, const AD& to_print)
    {
      out << to_print.value() << "_" << to_print.index();
      return out;
    }
  };

  template <typename T, size_t N>
  class AD_Function : public AD_Crtp<T, AD_Function<T, N>>
  {
   public:
    AD_TYPES(T);

    using differential_type = AD_Differential<T, N>;

   protected:
    value_type _f_value;
    differential_type _df_value;

   public:
    AD_Function(){};
    AD_Function(const value_type& f, const differential_type& df) : _f_value(f), _df_value(df) {}

    value_type
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
      out << "{ f=" << to_print.value() << "\tdf=[ " << to_print.df() << "] }";

      return out;
    }
  };

  //////////////////////////////////////////////////////////////////

  ///////////////
  // operator* //
  ///////////////
  //
  template <typename T, typename IMPL>
  inline auto operator*(const AD_Final_Value_Type_t<T> g0, const AD_Crtp<T, IMPL>& g1) noexcept
  {
    return AD_Function{g0 * g1.value(), g0 * g1.differential()};
  }
  template <typename T, typename IMPL>
  inline auto operator*(const AD_Crtp<T, IMPL>& g1, const AD_Final_Value_Type_t<T> g0) noexcept
  {
    return g0 * g1;
  }

  template <typename T, typename IMPL0, typename IMPL1>
  inline auto operator*(const AD_Crtp<T, IMPL0>& g0, const AD_Crtp<T, IMPL1>& g1) noexcept
  {
    return AD_Function(g0.value() * g1.value(),
                       g1.value() * g0.differential() + g0.value() * g1.differential());
  }
}
