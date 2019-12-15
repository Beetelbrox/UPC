#include <iostream>

#include "floorplanning_solver.h"


int main() {
  Floorplanning_problem fp_problem("instance_10.inp");

  Floorplanning_solver fp_solver(fp_problem);
  fp_solver.solve();

  return 0;
}
