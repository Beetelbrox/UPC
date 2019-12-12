#include <iostream>

#include "floorplanning_solver.h"


int main() {
  Floorplanning_problem fp_problem("small_fp.txt");

  for (const Floorplan &fp : fp_problem) {
    fp.print();
  }
  Floorplanning_solver fp_solver(fp_problem);
  fp_solver.solve();

  return 0;
}
