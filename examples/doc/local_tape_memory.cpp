#include "MissionImpossible/MissionImpossible.hpp"

using namespace MissionImpossible;

int
main()
{
  auto print_tape_size = [](auto msg) {
    std::cout << std::endl << ">>> " << msg << std::endl;
    std::cout << "statements : " << tape<double>().statement_size() << std::endl;
    std::cout << "memory     : " << tape<double>().memory_size() << std::endl;
  };

  print_tape_size("Initial tape state");

  AD<double> x0 = 2, x1 = 3, y;

  y = 4 * x0 + 2 * x1;

  auto grad = gradient(y);
  std::cout << y << ", " << grad[x0] << ", " << grad[x1] << std::endl;

  print_tape_size("After first computation and before local tape use");

  // Same computation but using a local tape
  {
    MissionImpossible_Tape<double> local_tape;

    AD<double> x0 = 2, x1 = 3, y;

    y = 4 * x0 + 2 * x1;

    auto grad = gradient(local_tape, y);  // <- here gradient use the local_tape

    std::cout << y << ", " << grad[x0] << ", " << grad[x1] << std::endl;

    print_tape_size("After local tape use");
  }

  print_tape_size("After local tape destruction");
}
