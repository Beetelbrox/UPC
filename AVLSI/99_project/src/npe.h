#ifndef _NPE_GUARD
#define _NPE_GUARD

#include <vector>
#include <map>

class NPE {
  std::vector<int> npe;
  std::vector<int> operand_pos;
  std::map<int, int> chains;

  void swap_operands(std::vector<int> &perturbation, size_t ix);
  void invert_chain(std::vector<int> &perturbation, size_t ix);
  int swap_operand_operator(std::vector<int> &perturbation, size_t ix, bool side);

public:
  static const int V = -1, H = -2;
  NPE(const std::vector<int> &seq);

  std::vector<int> generate_random_perturbation();

  void print();
};

#endif
