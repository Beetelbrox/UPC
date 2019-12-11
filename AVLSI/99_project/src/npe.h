#ifndef _NPE_GUARD
#define _NPE_GUARD

#include <vector>
#include <utility>
#include <memory>

class NPE {

  std::unique_ptr<int[]> _npe, _operand_pos;
  int *_limit;
  std::vector<std::pair<int, int>> _chains;

  int parse_npe(const int* npe_seq);

public:
  static const int V = -1, H = -2;

  NPE();
  NPE(const std::vector<int> &seq);
  NPE(const int* npe_seq, size_t length);

  int apply_perturbation(const std::pair<int, int> &p, bool parse=1);

  int* begin() const;
  int* end() const;
  std::size_t size() const;
  std::size_t n_operands() const;
  std::size_t n_operators() const;
  std::size_t n_chains() const;
  int get_element(size_t ix) const;
  int get_operand_pos(size_t ix) const;
  int get_chain_pos(size_t ix) const;
  int get_chain_length(size_t ix) const;
  void print(bool print_internals=0) const;
};

#endif
