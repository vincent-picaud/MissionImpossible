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
    f.tape().reverse(0, row.data());
    return row;
  }

  template <typename T>
  Tape_Vector<T>
  gradient(const AD<T>& f)
  {
    return Jacobian_row(f);
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
}
