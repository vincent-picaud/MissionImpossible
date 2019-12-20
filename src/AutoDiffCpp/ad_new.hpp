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
  template <typename T>
  class New_AD
  {
   public:
    using tape_type  = Tape<T>;
    using index_type = typename tape_type::index_type;
    using value_type = typename tape_type::value_type;

   protected:
    value_type _value;
    index_type _index;

   public:
    New_AD() noexcept {};  // avoid useless default init (zero filled double, int ... for instance)
    New_AD(const AD_Final_Value_Type_t<value_type> value) noexcept : New_AD() { (*this) = value; }

    New_AD&
    operator=(const AD_Final_Value_Type_t<value_type> value) noexcept
    {
      _value = value;
      _index = tape().add_variable();
      return *this;
    }

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
    operator<<(std::ostream& out, const New_AD& to_print)
    {
      out << to_print.value() << "_" << to_print.index();
      return out;
    }
  };
}
