#include "MissionImpossible/MissionImpossible.hpp"

using namespace MissionImpossible;

int
main()
{
  // GOOD
  //================
  AD<double> x1, y1;

  x1 = 1;       // initializes x1
  y1 = 2 * x1;  // before usage

  auto grad1 = gradient(y1);  // OK

  // BAD
  //================
  AD<double> x2, y2;

  y2 = 2 * x2;  // use of x2 without initialization
                // triggers an assert(0) in DEBUG mode

  auto grad2 = gradient(y2);  // undefined behavior
}
