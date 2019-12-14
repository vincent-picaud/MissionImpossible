#pragma once

#include "AutoDiffCpp/JamesBond_tape.hpp"
#include "AutoDiffCpp/ad.hpp"
#include "AutoDiffCpp/tape.hpp"
#include "AutoDiffCpp/tape_vector.hpp"

namespace AutoDiffCpp
{

  template<typename T>
  Tape_Vector<typename AD<T>::tape_type>
  gradient(const AD<T>& f)
  {
    
  }
}
