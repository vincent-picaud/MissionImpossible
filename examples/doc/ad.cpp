#include "MissionImpossible/MissionImpossible.hpp"

using namespace  MissionImpossible;

int main()
{
  AD<double> x,y;

  y = 2*x;

  gradient(y);
}
