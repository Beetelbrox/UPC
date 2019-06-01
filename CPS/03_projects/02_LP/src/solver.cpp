/*------------------------------------------------------------
 - Project: Linear Programming NLSP Solver
 - Subject: Combinatorial Problem Solving
 - Author:  Francisco Javier Jurado Moreno
 - Date:    June 2019
 - File:    solver.cpp
            MAIN
*///----------------------------------------------------------

#include <iostream>
#include <fstream>

#include "NLSP.h"

int main() {
  int num_inputs, tt_size;
  bool solution_found = false;
  // Redirect cout to stderr to prevent the solver from printing progress info
  // to it.
  std::streambuf *coutbuf = std::cout.rdbuf();
  std::cout.rdbuf(std::cerr.rdbuf());

  // Read the input from stdin
  // Use bit shift to calculate the number of truth table rows from the number
  // of inputs
  std::cin >> num_inputs;
  int tt [1 << num_inputs];
  for (int i=0; i < (1 << num_inputs); ++i) std::cin >> tt[i];

  // Search for solutions at increasing circuit depth.
  // At each iteration generate a new instance of NLSP for a given depth.
  for (int d=0; !solution_found; ++d) {
    std::cerr << "Searching for solution at circuit depth " << d << std::endl;
    NLSP nlsp(d, num_inputs, tt);
    nlsp.solve();
    if ( nlsp.solution_found() ) {
      solution_found = true;
      std::cerr << "Solution found at depth " << d << std::endl;
      nlsp.print_debug ();  // Print debug information to stderr
      // Restore cout to print the solution as requested
      std::cout.rdbuf(coutbuf);
      // Print a copy of the input data
      std::cout << num_inputs << std::endl;
      for (int i=0; i< (1 << num_inputs); ++i ) std::cout << tt[i] << std::endl;
      std::cout << d << " " << nlsp.get_objective_value() << std::endl;
      // Print the solution
      nlsp.print_solution();
      break;
    }
  }

  return 0;
}
