#ifndef _FP_SOLUTION_GUARD
#define _FP_SOLUTION_GUARD

#include "npe.h"

class Floorplanning_solution {
  // Data Structures
  std::vector<int> npe, operand_pos;
  std::map<int, int> chains;
  // Solution info
  int width, height;

  void swap_operands(std::vector<int> &perturbation, size_t ix);
  void invert_chain(std::vector<int> &perturbation, size_t ix);
  int swap_operand_operator(std::vector<int> &perturbation, size_t ix, bool side);

public:
  static const int V = -1, H = -2;
  Floorplanning_solution(const std::vector<int> &seq);

  std::vector<int> generate_random_perturbation();

  int get_width();
  int get_height();
  int get_num_chains();
  std::vector<int>& get_npe();

  void print();

};
#endif
