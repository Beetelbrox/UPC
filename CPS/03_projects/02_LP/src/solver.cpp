#include <iostream>
#include <vector>
#include <fstream>

#include "NLSP.h"

int main() {
  int num_inputs, tt_size;
  std::streambuf *coutbuf = std::cout.rdbuf();
  std::cout.rdbuf(std::cerr.rdbuf());

  // Read the number of inputs from stdin and calculate the truth table size
  std::cin >> num_inputs;
  // Read the truth table values from stdin
  int tt [1 << num_inputs];
  for (int i=0; i< (1 << num_inputs); ++i ) {
    std::cin >> tt[i];
  }

  for ( int d=0; d < 7; ++d ) {
    std::cerr << "Searching for solution at circuit depth " << d << std::endl;
    NLSP nlsp(d, num_inputs, tt);
    nlsp.solve();
    if ( nlsp.solution_found() ) {
      std::cerr << "Solution found at depth " << d << std::endl;
      nlsp.print_debug ();

      // Print solution
      std::cout.rdbuf(coutbuf);  // restore the regular output stream
      std::cout << num_inputs << std::endl;
      for (int i=0; i< (1 << num_inputs); ++i ) {
        std::cout << tt[i] << std::endl;
      }
      std::cout << d << " " << nlsp.get_objective_value() << std::endl;
      nlsp.print_solution();
      break;
    }
  }


  return 0;
}
