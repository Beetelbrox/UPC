/*########################################
 - File: npe.h
 - Author: Francisco Javier Jurado Moreno
 - Project: AVLSI Floorplanning project
##########################################*/
#ifndef _NPE_GUARD
#define _NPE_GUARD

#include <vector>
#include <utility>

class NPE {

  std::vector<int> _npe;
  std::vector<size_t> _operand_pos;
  std::vector<std::pair<size_t, size_t>> _chains;
  int _pert_type, _pert_side;
  size_t _pert_op1, _pert_op2, _pert_chain;
  std::pair<size_t, size_t> _pert;

  void _parse_npe();

  std::pair<size_t, size_t> gen_rnd_operand_swap();
  std::pair<size_t, size_t>  gen_rnd_chain_inversion();
  std::pair<size_t, size_t> gen_rnd_operand_operator_swap();

public:
  static const int V = -1, H = -2;

  NPE(size_t num_operands);
  NPE(const std::vector<int> &seq);
  NPE( const NPE& other );

  std::pair<size_t, size_t> gen_rnd_perturbation();
  void apply_perturbation();

  const int* begin() const;
  const int* end() const;
  int& operator[] (size_t ix);
  size_t size() const;
  int get_pert_element(size_t ix);
  size_t n_operands() const;
  size_t n_operators() const;
  size_t n_chains() const;
  //size_t get_operand_pos(size_t ix) const;
  //size_t get_chain_pos(size_t ix) const;
  //size_t get_chain_length(size_t ix) const;
  void print(bool print_internals=0) const;
};

#endif
