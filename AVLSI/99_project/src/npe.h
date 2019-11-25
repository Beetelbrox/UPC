#ifndef _NPE_GUARD
#define _NPE_GUARD

#include <vector>
#include <utility>
#include <memory>

class NPE {
  std::size_t length, num_operands, num_operators;
  std::unique_ptr<int[]> npe, operand_pos;
  std::vector<std::pair<int, int>> chains;

  int parse_npe();
  std::pair<int, int> gen_rnd_operand_swap();
  std::pair<int, int> gen_rnd_chain_swap();
  std::pair<int, int> gen_rnd_operand_operator_swap();

public:
  static const int V = -1, H = -2;
  NPE(const std::vector<int> &seq);


  std::pair<int, int> gen_rnd_perturbation();
  int apply_perturbation(const std::pair<int, int> &perturbation, bool parse=true);

  std::size_t get_length();
  std::size_t get_num_operands();
  std::size_t get_num_operators();
  void print();
};

#endif
