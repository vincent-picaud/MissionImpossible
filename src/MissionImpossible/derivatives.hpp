#pragma once

#include "MissionImpossible/ad.hpp"
#include "MissionImpossible/mission_impossible_tape.hpp"
#include "MissionImpossible/tape.hpp"
#include "MissionImpossible/tape_vector.hpp"

namespace MissionImpossible
{
  //////////////////////////////////////////////////////////////////
  // Row by row (or reverse mode)
  //////////////////////////////////////////////////////////////////
  //
  template <typename T>
  Tape_Vector<T>
  Jacobian_row(const AD<T>& f)
  {
    Tape_Vector<T> row(tape<T>());
    row.assign_ei(f);
    f.tape().reverse(0, row.data());
    return row;
  }
  // Note:
  //
  // The recursive implementation of MissionImpossible_Tape<...> is
  // there to support syntactic sugar for nested case
  //
  // By example, it allows:
  //
  // auto Hessian_row = gradient(grad[x])
  //
  // instead of
  //
  // auto Hessian_row = gradient(grad[x].value())
  //
  template <typename T>
  Tape_Vector<T>
  Jacobian_row(const MissionImpossible_Tape<typename AD<T>::value_type>& tape, const AD<T>& f)
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
  gradient(const MissionImpossible_Tape<typename AD<T>::value_type>& tape, const AD<T>& f)
  {
    return Jacobian_row(tape, f);
  }

  //////////////////////////////////////////////////////////////////
  // Column by column (or forward mode)
  //////////////////////////////////////////////////////////////////
  //
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
  Jacobian_column(const MissionImpossible_Tape<typename AD<T>::value_type>& tape, const AD<T>& f)
  {
    Tape_Vector<T> column(tape);
    column.assign_ei(f);
    f.tape().forward(tape.index_begin(), column.data() - tape.index_begin());
    return column;
  }
}
