#include <iostream>
#include <unordered_set>
#include <numeric>      // For iota
#include <algorithm>    // For random_shuffle
#include "npe.h"

using namespace std;

NPE::NPE(size_t num_operands, bool shuffle):
  _npe(2*num_operands-1)
{
  int operands[num_operands];

  // Create a sequence of indices and shuffle it if requested
  iota(operands, &operands[num_operands], 1);
  if(shuffle) random_shuffle(operands, &operands[num_operands]);

  bool next_insert = 0;
  for ( size_t i=0; i < _npe.size(); ++i ) {
    if ( _operand_pos.size() >= num_operands ) next_insert = 1; // All operands already inserted
    else if ( i + 1 >= 2*_operand_pos.size()) next_insert = 0;  // Balloting rule
    else next_insert = rand()%2;

      if (next_insert) {
    if (_npe[i-1])
  }
  }



  for(int* npe_it = npe_seq.get(); npe_it != &npe_seq[npe_size]; ++npe_it) {
    if (op_it-operands.get() >= int(n_operands)) next_insert = 1;       // All operands inserted
    else if ( npe_it - npe_seq.get() + 1 >= 2*(op_it-operands.get()) ) next_insert = 0;  // Balloting rule
    else next_insert = rand()%2;

    // 1 insert operator, 0 insert operand
    if(next_insert) *npe_it = *(npe_it-1) < 0 ? !(*(npe_it-1) + 2) - 2 : (rand()%2)-2;
    else *npe_it =  *(op_it++);
  }
  npe = NPE(npe_seq.get(), npe_size);
  cerr << "done." << endl;
}
}

NPE::NPE(const vector<int> &npe_seq): NPE(&npe_seq[0], npe_seq.size()) {}

NPE::NPE(const int* npe_seq, size_t length): _operand_pos{nullptr}{
  _npe = make_unique<int[]>(length);
  _limit = _npe.get() + length;
  _operand_pos = make_unique<int[]>(n_operands());
  if(!parse_npe(&npe_seq[0])) copy(npe_seq, npe_seq + length, _npe.get()); // If the parsing succeeds, copy the sequence to the internal structure
  else {
    _npe.reset(nullptr);
    _limit = nullptr;
    _operand_pos.reset(nullptr);
  }
}

void Floorplanning_solver::generate_random_npe(size_t n_operands, NPE& npe, bool shuffle) {
  cerr << "Generating random solution...";
  size_t npe_size = 2*n_operands-1;
  // Allocate the arrays in the heap to avoid stack overflows
  unique_ptr<int[]> npe_seq = make_unique<int[]>(npe_size), operands = make_unique<int[]>(n_operands);
  int *op_it = operands.get(), next_insert;

  // Create a sequence of indices and shuffle it if requested
  iota(op_it, &op_it[n_operands], 1);
  if(shuffle) random_shuffle(op_it, &op_it[n_operands]);

  for(int* npe_it = npe_seq.get(); npe_it != &npe_seq[npe_size]; ++npe_it) {
    if (op_it-operands.get() >= int(n_operands)) next_insert = 1;       // All operands inserted
    else if ( npe_it - npe_seq.get() + 1 >= 2*(op_it-operands.get()) ) next_insert = 0;  // Balloting rule
    else next_insert = rand()%2;

    // 1 insert operator, 0 insert operand
    if(next_insert) *npe_it = *(npe_it-1) < 0 ? !(*(npe_it-1) + 2) - 2 : (rand()%2)-2;
    else *npe_it =  *(op_it++);
  }
  npe = NPE(npe_seq.get(), npe_size);
  cerr << "done." << endl;
}





int* NPE::begin() const { return _npe.get(); }

int* NPE::end() const { return _limit; }

int& NPE::operator[] (int ix) {
  if ( ix >= size() ) {
    cerr << "NPE Error: Index out of bounds" << endl;
    exit(EXIT_FAILURE);
  }
  return _npe[ix];
}

size_t NPE::size() const { return _limit - _npe.get(); }

size_t NPE::n_operands() const { return (_limit-_npe.get()+1)>>1; }

size_t NPE::n_operators() const { return (_limit - _npe.get())>>1; }

int NPE::get_element(size_t ix) const { return _npe[ix]; }

int NPE::parse_npe(const int* npe_seq){
  // Temporary data structures
  unique_ptr<int[]> operand_pos = make_unique<int[]>(n_operands());   // Allocate this on the heap to make it resilient to Stack overflow
  vector<pair<int, int>> chains;
  size_t operand_ctr = 0, operator_ctr = 0;
  unordered_set <size_t> rd_operands; // Hash table to check for already inserted elements

  for(size_t i=0; i < size(); ++i) {
    if (
      npe_seq[i] > 0 &&                                       // If the value is a block id:
      npe_seq[i] <= int(n_operands()) &&                      //  1. Check that is within the bounds (as we will use the id to index the floorplan)
      rd_operands.find(npe_seq[i]) == rd_operands.end() &&    //  2. Check that we have not already read that module id
      operand_ctr < n_operands()                              //  3. Check that the number of read operands has not reached the expected amount of operands
    ){
      operand_pos[operand_ctr++] = i;
      rd_operands.insert(npe_seq[i]);                         // Add the block id to the read set

    } else if (
      (npe_seq[i] == V || npe_seq[i] == H) &&                 // If the value is a slice operator:
      operator_ctr + 1 < operand_ctr  &&                      //  1. Check that the NPE satisfies the balloting rule
      npe_seq[i-1] != npe_seq[i] &&                           //  2. Check that the NPE is indeed normalized (condition 1 prevents operator for being on the first place, so this is safe)
      operator_ctr < n_operators()                            //  3. Check that the number of read operators has not exceeded the expected amount of operands (not sure if this is ever broken without breaking anything else before)
    ) {
      if (npe_seq[i-1] > 0) chains.emplace_back(i, 1);       // If the element is the head of a new chain, push it into the vector,
      else ++chains.back().second;                           // oitherwise increase the length on the current chain
      ++operator_ctr;

    } else break; // If we reach this state, the input is malformed
  }

  // If we have read the expected number of operators and operands,
  // write the new indices on top of the NPE ones
  if (operand_ctr == n_operands() && operator_ctr == n_operators()) {
    _chains = chains;
    copy(&operand_pos[0], &operand_pos[0]+operand_ctr, _operand_pos.get());
    return 0;
  }
  cerr << "Error: Malformed NPE sequence" << endl;
  return -1;
}

int NPE::apply_perturbation(const pair<int, int> &p, bool parse) {
  if (p.first < 0 || p.first >= int(size())) return -1; // Out of bounds
  if (p.second < 0 && (_npe[p.first] == V || _npe[p.first] == H)) {
    for(size_t i=p.first; i < size(); ++i) {
      if (_npe[i] >= 0) return 0;
      _npe[i] = (_npe[i] == V) ? H : V;
    }
  } else if (p.second >=0 && p.second < int(size())){
    swap(_npe[p.first], _npe[p.second]);
    if(!parse) return 0;                    // If parsing is disabled we're done
    if(!parse_npe(_npe.get())) return 0;              // If the parsing succeeds, we're done
    apply_perturbation(p, false);           // If the parsing fails, undo the perturbation without checking for correctness
  }
  return -1;                                // If it reaches this point, return error state
}

void NPE::print(bool print_internals) const{
  for (int* it=_npe.get(); it != _limit; ++it) {
    if (*it == V) cerr << "V";
    else if (*it == H) cerr << "H";
    else cerr << *it;
    cerr << " ";
  }
  cout << endl;
  if (print_internals) {
    cerr << "Operand positions:" << endl;
    for(size_t i=0; i < n_operands(); ++i) cerr << _operand_pos[i] << " ";
    cerr << endl << "Operator chains:" << endl;
    for(const pair<int, int>& p : _chains) cerr << "(" << p.first << "," << p.second << ") ";
    cerr << endl;
  }
}

size_t NPE::n_chains() const { return _chains.size(); }
int NPE::get_operand_pos(size_t ix) const { return _operand_pos[ix]; }
int NPE::get_chain_pos(size_t ix) const { return _chains[ix].first; }
int NPE::get_chain_length(size_t ix) const { return _chains[ix].second; }
const int* NPE::get_npe_seq() const { return _npe.get(); }
