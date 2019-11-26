#ifndef _NPE_GUARD
#define _NPE_GUARD

#include <vector>
#include <utility>
#include <memory>
#include <cstddef>

class NPE {

  std::unique_ptr<int[]> _npe, _operand_pos;
  int *_limit;
  std::size_t _n_operands, _n_operators;
  std::vector<std::pair<int, int>> _chains;

  int parse_npe();
  std::pair<int, int> gen_rnd_operand_swap();
  std::pair<int, int> gen_rnd_chain_swap();
  std::pair<int, int> gen_rnd_operand_operator_swap();

public:
  static const int V = -1, H = -2;
  NPE();
  NPE(const std::vector<int> &seq);
  NPE(const int* npe_seq, size_t length);


  std::pair<int, int> gen_rnd_perturbation();
  int apply_perturbation(const std::pair<int, int> &perturbation, bool parse=true);



  std::size_t size();
  std::size_t get_num_operands();
  std::size_t get_num_operators();
  int get_element(size_t ix);
  void print();
};

#endif
