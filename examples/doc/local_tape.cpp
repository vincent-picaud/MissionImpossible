#include "MissionImpossible/MissionImpossible.hpp"

using namespace MissionImpossible;

int
main()
{
  auto print_tape_size = []() {
    std::cout << "size: " << tape<double>().statement_size() << std::endl;
  };

  // GOOD
  //================
  print_tape_size();

  {
    MissionImpossible_Tape<double> local_tape;

    AD<double> x0 = 2, x1 = 3, y;

    y = 4 * x0 + 2 * x1;

    auto grad = gradient(local_tape, y);
    std::cout << y << ", " << grad[x0] << ", " << grad[x1] << std::endl;
    print_tape_size();
  }

  print_tape_size();

  // GOOD
  //================
  AD<double> a = 1, b = 2;  // Ok, as a, b are not used in local_tape scope

  print_tape_size();
  {
    MissionImpossible_Tape<double> local_tape;

    AD<double> x0 = 2, x1 = 3, y;

    y = 4 * x0 + 2 * x1;

    auto grad = gradient(local_tape, y);
    std::cout << y << ", " << grad[x0] << ", " << grad[x1] << std::endl;
    print_tape_size();
  }

  print_tape_size();
}
