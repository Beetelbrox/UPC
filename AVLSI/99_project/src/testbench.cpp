#include <iostream>

#include "floorplanning_problem.h"


int main() {
  //Floorplanning_problem fp_problem("testfile.txt");
  //fp_problem.load_from_file("testfile.txt");
  //Floorplanning_solver fp_solver(fp_problem);
  //fp_solver.solve();

  Floorplanning_problem fp_problem;
  std::cout << fp_problem.size() << std::endl;
  if (!fp_problem.from_file("testfile.txt")) {
    for(Floorplan &fp : fp_problem) { // Use reference as a compy constructor is not implemented
      fp.print();
    }

    for(size_t i=0; i < fp_problem.size(); ++i) {
      fp_problem.get_floorplan(i)->print();
    }
  }
  return 0;
}
