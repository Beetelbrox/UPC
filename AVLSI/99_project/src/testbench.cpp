#include <iostream>

#include "floorplanning_solver.h"


int main() {
  Floorplanning_problem fp_problem("testfile.txt");
  Floorplanning_solver fp_solver(fp_problem);

  return 0;
}
