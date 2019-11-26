#include <iostream>

#include "npe.h"


int main() {
  NPE empty_npe;
  std::cout << empty_npe.size() << " " << empty_npe.n_operands() << " " << empty_npe.n_operators() << std::endl;
  empty_npe.print();

  NPE small_npe({1, 2, NPE::V, 3, 4, NPE::V, NPE::H});
  small_npe.print(true);
  std::cout << small_npe.size() << " " << small_npe.n_operands() << " " << small_npe.n_operators() << std::endl;

  for (int e : small_npe) std::cout << e << " ";
  std::cout << std::endl;

  std::cout << small_npe.apply_perturbation({5,4}) << std::endl;
  small_npe.print(true);

  return 0;
}
