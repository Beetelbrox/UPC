#include <iostream>

#include "floorplanning_solver.h"
#include "npe.h"


int main() {
  Floorplanning_problem fp_problem("testfile.txt");
  Floorplanning_solver fp_solver(fp_problem);
  fp_solver.solve();
  return 0;
}
