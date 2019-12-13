#ifndef _NPE_GUARD
#define _NPE_GUARD

#include <vector>
#include <utility>

class NPE {

  std::vector<int> _npe;
  std::vector<size_t> _operand_pos;
  std::vector<std::pair<size_t, size_t>> _chains;

  void _parse_npe();

public:
  static const int V = -1, H = -2;

  NPE(size_t num_operands, bool shuffle=0);
  NPE(const std::vector<int> &seq);

  int apply_perturbation(const std::pair<int, int> &p, bool parse=1);

  const int* begin() const;
  const int* end() const;
  int& operator[] (int ix);
  size_t size() const;
  size_t n_operands() const;
  size_t n_operators() const;
  size_t n_chains() const;
  size_t get_operand_pos(size_t ix) const;
  size_t get_chain_pos(size_t ix) const;
  size_t get_chain_length(size_t ix) const;
  void print(bool print_internals=0) const;
};

#endif
