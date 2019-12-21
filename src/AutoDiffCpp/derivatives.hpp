#pragma once

//#include "AutoDiffCpp/JamesBond_tape.hpp"
#include "AutoDiffCpp/ad.hpp"
#include "AutoDiffCpp/tape.hpp"
#include "AutoDiffCpp/tape_vector.hpp"

namespace AutoDiffCpp
{
  template <typename T>
  Tape_Vector<T>
  Jacobian_row(const AD<T>& f)
  {
    Tape_Vector<T> row(f.tape().row_size());
    row.assign_ei(f.index());
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
    Tape_Vector<T> column(f.tape().row_size());
    column.assign_ei(f.index());
    f.tape().forward(0, column.data());
    return column;
  }
}
