#include <iostream>

#include "floorplanning_solver.h"
#include "floorplanning_problem.h"
#include "npe.h"


int main() {
  NPE npe({1, 2, 3, -2, -1, 4, -1});
  npe.print();
  for(int i=0; i <1000; ++i) {
    std::pair<int, int> perturbation = npe.gen_rnd_perturbation();
    std::cout << "(" << perturbation.first << "," << perturbation.second << ")" << std::endl;
    npe.apply_perturbation(perturbation);
    npe.print();
  }

  return 0;
}
