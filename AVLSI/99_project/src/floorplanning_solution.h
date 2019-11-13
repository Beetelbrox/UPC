#ifndef _FP_SOLUTION_GUARD
#define _FP_SOLUTION_GUARD

#include <vector>
#include <map>

class Floorplanning_solution {
  std::map<int, int> operands;
  std::map<int, std::vector<int> > chains;
  int width, height;
public:
  Floorplanning_solution(const std::vector<int> &npe);

  void swap_operands(int ix);
  void invert_chain(int ix);
  int swap_operand_operator(int ix);

  int get_width();
  int get_height();
  int get_num_chains();

  void print_npe();
};
#endif
