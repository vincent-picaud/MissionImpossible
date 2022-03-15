#include "MissionImpossible/identity.hpp"

#include <complex>

namespace MissionImpossible
{
  ///////////////////////////////////////////////
  // Add mixed-type arithmetic to std::complex //
  ///////////////////////////////////////////////
  //

  // Comparison operators
  //////////////////////////////////////////////////////////////////
  //
  template <typename T>
  bool
  operator==(const std::complex<T>& c, const Identity_t<T>& s)
  {
    return (c.imag() == 0) && (c.real() == s);
  }
  template <typename T>
  bool
  operator==(const Identity_t<T>& s, const std::complex<T>& c)
  {
    return c == s;
  }

  template <typename T>
  bool
  operator!=(const std::complex<T>& c, const Identity_t<T>& s)
  {
    return (c.imag() != 0) || (c.real() != s);
  }

  template <typename T>
  bool
  operator!=(const Identity_t<T>& s, const std::complex<T>& c)
  {
    return c != s;
  }

  // Arithmetic operators
  //////////////////////////////////////////////////////////////////
  //
  template <typename T>
  auto operator*(const std::complex<T>& c, const Identity_t<T>& s)
  {
    return c * s;
  }

  template <typename T>
  auto operator*(const Identity_t<T>& s, const std::complex<T>& c)
  {
    return s * c;
  }

  template <typename T>
  auto
  operator/(const std::complex<T>& c, const Identity_t<T>& s)
  {
    return c / s;
  }

  template <typename T>
  auto
  operator/(const Identity_t<T>& s, const std::complex<T>& c)
  {
    return s / c;
  }

  template <typename T>
  auto
  operator+(const std::complex<T>& c, const Identity_t<T>& s)
  {
    return c + s;
  }

  template <typename T>
  auto
  operator+(const Identity_t<T>& s, const std::complex<T>& c)
  {
    return s + c;
  }

  template <typename T>
  auto
  operator-(const std::complex<T>& c, const Identity_t<T>& s)
  {
    return c - s;
  }

  template <typename T>
  auto
  operator-(const Identity_t<T>& s, const std::complex<T>& c)
  {
    return s - c;
  }
}
