#include "MissionImpossible/MissionImpossible.hpp"

using namespace MissionImpossible;

int
main()
{
  auto print_tape_size = [](auto msg) {
    std::cout << std::endl << ">>> " << msg << std::endl;
    std::cout << "statements                : " << tape<double>().statement_size() << std::endl;
    std::cout << "memory           (kBytes) : " << tape<double>().memory_size()/1024 << std::endl;
    std::cout << "allocated memory (kBytes) : " << tape<double>().allocated_memory_size()/1024 << std::endl;
  };

  print_tape_size("Initial tape state (contains a small amount of preallocated memory)");

  for (size_t i = 1; i < 1000; ++i)
  {
    AD<double> x0 = 2, x1 = 3, y;

    y = 4 * x0 + 2 * x1;

    auto grad = gradient(y);
  }

  print_tape_size("Final tape state (tape has allocated some fresh memory)");

  tape<double>().reset();
  print_tape_size("after tape.reset() (the extra allocated memory is not released)");

  tape<double>().clear();
  print_tape_size("after tape.clear() (releases extra memory and starts with a new tape)");
}
