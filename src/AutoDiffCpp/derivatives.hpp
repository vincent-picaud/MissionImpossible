#pragma once

#include "AutoDiffCpp/ad.hpp"
#include "AutoDiffCpp/mission_impossible_tape.hpp"
#include "AutoDiffCpp/tape.hpp"
#include "AutoDiffCpp/tape_vector.hpp"

namespace AutoDiffCpp
{
  template <typename T>
  Tape_Vector<T>
  Jacobian_row(const AD<T>& f)
  {
    Tape_Vector<T> row(tape<T>());
    row.assign_ei(f);
    f.tape().reverse(0, row.data());
    return row;
  }
  template <typename T>
  Tape_Vector<T>
  Jacobian_row(const Mission_Impossible_Tape<T>& tape, const AD<T>& f)
  {
    Tape_Vector<T> row(tape);
    row.assign_ei(f);
    f.tape().reverse(tape.index_begin(), row.data() - tape.index_begin());
    return row;
  }

  template <typename T>
  Tape_Vector<T>
  gradient(const AD<T>& f)
  {
    return Jacobian_row(f);
  }

  template <typename T>
  Tape_Vector<T>
  gradient(const Mission_Impossible_Tape<T>& tape, const AD<T>& f)
  {
    return Jacobian_row(tape, f);
  }
  // Syntaxic sugar for nested case
  //
  // Allows:
  //
  // auto Hessian_row = gradient(grad[x])
  //
  // intead of
  //
  // auto Hessian_row = gradient(grad[x].value())
  //
  template <typename T, typename U, typename = std::enable_if_t<Is_AD_v<U>>>
  Tape_Vector<T>
  gradient(const Mission_Impossible_Tape<U>& tape, const AD<T>& f)
  {
    return gradient(static_cast<const Mission_Impossible_Tape<typename U::value_type>&>(tape), f);
  }

  //================

  template <typename T>
  Tape_Vector<T>
  Jacobian_column(const AD<T>& f)
  {
    Tape_Vector<T> column(tape<T>());
    column.assign_ei(f);
    f.tape().forward(0, column.data());
    return column;
  }

  template <typename T>
  Tape_Vector<T>
  Jacobian_column(const Mission_Impossible_Tape<T>& tape, const AD<T>& f)
  {
    Tape_Vector<T> column(tape);
    column.assign_ei(f);
    f.tape().forward(tape.index_begin(), column.data() - tape.index_begin());
    return column;
  }
}
