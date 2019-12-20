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
  template <typename T, size_t N>
  struct AD_Function;
  template <typename T, size_t N>
  struct AD_Differential;

  template <typename T>
  class AD
  {
   public:
    using tape_type  = Tape<T>;
    using index_type = typename tape_type::index_type;
    using value_type = typename tape_type::value_type;

   protected:
    value_type _value;
    index_type _index;

   public:
    AD() noexcept {};  // avoid useless default init (zero filled double, int ... for instance)
    AD(const AD_Final_Value_Type_t<value_type> value) noexcept : AD() { (*this) = value; }

    AD&
    operator=(const AD_Final_Value_Type_t<value_type> value) noexcept
    {
      _value = value;
      _index = tape().add_variable();
      return *this;
    }

    operator AD_Function<T, 1>() const { return {_value, AD_Differential<T, 1>{{T(1)}, {_index}}}; }
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
    const index_type&
    index() const noexcept
    {
      return _index;
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

    friend std::ostream&
    operator<<(std::ostream& out, const AD& to_print)
    {
      out << to_print.value() << "_" << to_print.index();
      return out;
    }
  };

  template <typename T, std::size_t N>
  class AD_Differential
  {
   public:
    using tape_type        = Tape<T>;
    using index_type       = typename tape_type::index_type;
    using value_type       = typename tape_type::value_type;
    using value_array_type = std::array<T, N>;
    using index_array_type = std::array<index_type, N>;

   protected:
    value_array_type _value_array;
    index_array_type _index_array;

   public:
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

  template <typename T, size_t N>
  struct AD_Function
  {
    using tape_type = Tape<T>;
    //    using index_type = typename tape_type::index_type;
    using value_type = typename tape_type::value_type;

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

  //////////////////////////////////////////////////////////////////

  ///////////////
  // operator* //
  ///////////////
  //
  template <typename T, size_t N>
  inline auto operator*(const AD_Final_Value_Type_t<T> g0, const AD_Function<T, N>& g1) noexcept
  {
    return chain_rule(g0 * g1.value(), g0, g1);
  }
}
