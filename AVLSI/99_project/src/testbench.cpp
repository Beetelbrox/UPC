#include <iostream>

#include "floorplanning_solver.h"


int main() {
  Floorplanning_problem fp_problem("small_fp.txt");
  Floorplanning_solver fp_solver(fp_problem);

  return 0;
}
