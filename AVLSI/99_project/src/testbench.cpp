#include <iostream>

#include "floorplanning_solver.h"


int main() {
  Floorplanning_problem problem("testfile.txt");
  Floorplanning_solver solver(problem);
  solver.solve();
  return 0;
}
