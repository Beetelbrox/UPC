#include <iostream>
#include <unordered_set>
#include <numeric>      // For iota
#include <algorithm>    // For random_shuffle
#include "npe.h"

using std::cerr;
using std::endl;
using std::vector;
using std::pair;

NPE::NPE(size_t num_operands, bool shuffle):
  _npe(2*num_operands-1),
  _operand_pos(num_operands)
{
  int operands[num_operands], op_counter=0;

  // Create a sequence of indices and shuffle it if requested
  std::iota(operands, &operands[num_operands], 1);
  if(shuffle) std::random_shuffle(operands, &operands[num_operands]);

  bool next_insert = 0;
  for ( size_t i=0; i < _npe.size(); ++i ) {
    if ( op_counter >= num_operands ) next_insert = 1; // All operands already inserted
    else if ( i+1 >= 2*op_counter) next_insert = 0;  // Balloting rule
    else next_insert = rand()%2;

    if (next_insert) {
      if (_npe[i-1] < 0 ){
        _npe[i] = (_npe[i-1] == V) ? H : V;
        _chains.back().second++;
      } else {
        _npe[i] = (rand()%2) - 2;
        _chains.push_back( {i, 1} );
      } 
    } else {
      _npe[i] = operands[op_counter];
      _operand_pos[operands[op_counter]-1] = i;
      ++op_counter;
    }
  }
}

NPE::NPE(const vector<int> &npe_seq): _npe(npe_seq) {
  _parse_npe();
}

void NPE::_parse_npe(){
  // Temporary data structures
  _operand_pos = vector<size_t>(n_operands(), 0);
  vector<bool> read_op(n_operands(), 0);
  int op_counter = 0;

  for(size_t i=0; i < _npe.size(); ++i) {
    if (_npe[i] > 0 ) {                                         // If the value is a block id:
      if( _npe[i] <= int( n_operands() ) ) exit(EXIT_FAILURE);  //  1. Check that is within the bounds (as we will use the id to index the floorplan)
      if( read_op[_npe[i]-1] ) exit(EXIT_FAILURE);              //  2. Check that we have not already read that module id
      if( op_counter >= n_operands() ) exit(EXIT_FAILURE);      //  3. Check that the number of read operands has not reached the expected amount of operands
        _operand_pos[_npe[i]-1] = i;
        read_op[_npe[i]-1] = 1;
        ++op_counter;
    } else if (_npe[i] == V || _npe[i] == H) {                  // If the value is a slice operator:
      if ( i+1 < 2*op_counter ) exit(EXIT_FAILURE);             //  1. Check that the NPE satisfies the balloting rule
      if (_npe[i-1] == _npe[i] ) exit(EXIT_FAILURE);            //  2. Check that the NPE is indeed normalized (condition 1 prevents operator for being on the first place, so this is safe)
      if (_npe[i-1] > 0) _chains.emplace_back(i, 1);            // If the element is the head of a new chain, push it into the vector,
      else _chains.back().second++;                             // oitherwise increase the length on the current chain

    } else {
      cerr << "Error [NPE]: Malformed NPE sequence while parsing" << endl;
      exit(EXIT_FAILURE);
    }
  }
}

const int* NPE::begin() const { return &*_npe.begin(); }

const int* NPE::end() const { return &*_npe.end(); }

int& NPE::operator[] (int ix) {
  if ( ix >= size() ) {
    cerr << "NPE Error: Index out of bounds" << endl;
    exit(EXIT_FAILURE);
  }
  return _npe[ix];
}

size_t NPE::size() const { return _npe.size(); }
size_t NPE::n_operands() const { return (_npe.size()+1)>>1; }
size_t NPE::n_operators() const { return _npe.size() - n_operands(); }
size_t NPE::n_chains() const { return _chains.size(); }
size_t NPE::get_operand_pos(size_t ix) const { return _operand_pos[ix]; }
size_t NPE::get_chain_pos(size_t ix) const { return _chains[ix].first; }
size_t NPE::get_chain_length(size_t ix) const { return _chains[ix].second; }



int NPE::apply_perturbation(const pair<int, int> &p, bool parse) {
  if (p.first < 0 || p.first >= int(size())) return -1; // Out of bounds
  if (p.second < 0 && (_npe[p.first] == V || _npe[p.first] == H)) {
    for(size_t i=p.first; i < size(); ++i) {
      if (_npe[i] >= 0) return 0;
      _npe[i] = (_npe[i] == V) ? H : V;
    }
  } else if (p.second >=0 && p.second < int(size())){
    std::swap(_npe[p.first], _npe[p.second]);
    if(!parse) return 0;                    // If parsing is disabled we're done
    //if(!parse_npe(_npe.get())) return 0;              // If the parsing succeeds, we're done
    apply_perturbation(p, false);           // If the parsing fails, undo the perturbation without checking for correctness
  }
  return -1;                                // If it reaches this point, return error state
}

void NPE::print(bool print_internals) const{
  for (int e : _npe) {
    if (e == V) cerr << "V";
    else if (e == H) cerr << "H";
    else cerr << e;
    cerr << " ";
  }
  cerr << endl;
  if (print_internals) {
    cerr << "Operand positions:" << endl;
    for(size_t i=0; i < n_operands(); ++i) cerr << _operand_pos[i] << " ";
    cerr << endl << "Operator chains:" << endl;
    for(const pair<int, int>& p : _chains) cerr << "(" << p.first << "," << p.second << ") ";
    cerr << endl;
  }
}