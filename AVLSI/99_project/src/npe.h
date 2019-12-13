#ifndef _NPE_GUARD
#define _NPE_GUARD

#include <vector>
#include <utility>
#include <memory>

class NPE {

  std::vector<int> _npe, _operand_pos;
  std::vector<std::pair<int, int>> _chains;

  int parse_npe_seq( const vector<int> &npe_seq );

public:
  static const int V = -1, H = -2;

  NPE(size_t num_operands, bool shuffle=0);
  NPE(const std::vector<int> &seq);

  int apply_perturbation(const std::pair<int, int> &p, bool parse=1);

  int* begin() const;
  int* end() const;
  int& operator[] (int ix);
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
