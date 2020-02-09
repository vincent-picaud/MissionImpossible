#include "MissionImpossible/MissionImpossible.hpp"

using namespace MissionImpossible;

int
main()
{
  // GOOD
  //================
  {
    MissionImpossible_Tape<double> local_tape;

    AD<double> x0 = 2, x1 = 3, y;

    y = 4 * x0 + 2 * x1;

    auto grad = gradient(local_tape, y);
  }

  // GOOD
  //================
  AD<double> a = 1;  // Ok, as "a" is not used in local_tape scope

  {
    MissionImpossible_Tape<double> local_tape;

    AD<double> x0 = 2, x1 = 3, y;

    y = 4 * x0 + 2 * x1;

    auto grad = gradient(local_tape, y);
  }

  //  BAD
  //================
  {
    MissionImpossible_Tape<double> local_tape;

    AD<double> x0 = 2, x1 = 3, y;

    y = 4 * x0 + 2 * x1 + a;  // CAVEAT: "a" was not declared in the tape scope

    auto grad = gradient(local_tape, y);  // Undefined behavior. In
					  // DEBUG mode triggers an
					  // assert(0)
  }
}
