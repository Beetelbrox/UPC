/*########################################
 - File: npe.cpp
 - Author: Francisco Javier Jurado Moreno
 - Project: AVLSI Floorplanning project
##########################################*/

#include <iostream>
#include "npe.h"

using std::cerr;
using std::endl;
using std::vector;
using std::pair;

// Constructor generating a random npe with n operators
NPE::NPE(size_t num_operands):
  _npe(2*num_operands-1),
  _operand_pos(num_operands),
  _pert_type(-1)
{
 _npe[0] = 1;
 int n_operands = 1;
 for (size_t i=1; i < _npe.size();++i) {
   if (i%2) {
    _operand_pos[n_operands] = i;
    _npe[i] = ++n_operands;
   } else {
    _chains.emplace_back(i, 1);
    _npe[i] = V;
   }
 }
}

NPE::NPE(const vector<int> &npe_seq): _npe(npe_seq), _pert_type(-1) {
  _parse_npe();
}

NPE::NPE( const NPE& other ):
  _npe(other._npe),
  _operand_pos(other._operand_pos),
  _chains(other._chains),
  _pert_type(-1) {}

// Parse the npe and populate the _chains and _operand_pos
void NPE::_parse_npe(){
  // Temporary data structures
  _operand_pos = vector<size_t>(n_operands(), 0);
  vector<bool> read_op(n_operands(), 0);
  size_t op_counter = 0;

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
      else _chains.back().second++;                             // Otherwise increase the length on the current chain

    } else {
      cerr << "Error [NPE]: Malformed NPE sequence while parsing" << endl;
      exit(EXIT_FAILURE);
    }
  }
}

// [] Operator allows for convenient random access to the NPE
int& NPE::operator[] (size_t ix) {
  if ( ix >= size() ) {
      cerr << "Error [NPE]: Index out of bounds" << endl;
      exit(EXIT_FAILURE);
  }
  return _npe[ix];
}

const int* NPE::begin() const { return &*_npe.begin(); }
const int* NPE::end() const { return &*_npe.end(); }

size_t NPE::size() const { return _npe.size(); }
size_t NPE::n_operands() const { return (_npe.size()+1)>>1; }
size_t NPE::n_operators() const { return _npe.size() - n_operands(); }
size_t NPE::n_chains() const { return _chains.size(); }

// Returned the element at position ix of the perturbed NPE without actually applying the perturbation
int NPE::get_pert_element(size_t ix) {
  if ( ix >= size() ) {
      cerr << "Error [NPE]: Index out of bounds" << endl;
      exit(EXIT_FAILURE);
  }
  int pert_val;
  if (_pert_type < 0) pert_val =  _npe[ix];
  else if (_pert_type == 0 || _pert_type == 2) {
    if (ix == _pert.first ) pert_val =  _npe[_pert.second];
    else if (ix == _pert.second ) pert_val =  _npe[_pert.first];
    else pert_val = _npe[ix];
  } else if (_pert_type == 1) {
    if(ix >= _chains[_pert_chain].first && ix < _chains[_pert_chain].first + _chains[_pert_chain].second) {
      pert_val = (_npe[ix] == V) ? H : V;
    } else pert_val = _npe[ix];
  }
  return pert_val;
}

// Applies the stored perturbation to the NPE
// Swap for third case does inefficient reads (1 sweep of the operator list) in one of the cases, 
// but it's better than remaking all the structures 
void NPE::apply_perturbation() {
  if (_pert_type < 0) return;
  if(_pert.first < _npe.size() && _pert.second < _npe.size() ) {
    if(_pert_type == 0) {                             // Operand-operand swap
      std::swap(_npe[_pert_op1], _npe[_pert_op2]);  
    } else if (_pert_type == 1){                      // Chain inversion
      for( size_t i=_chains[_pert_chain].first; i <_chains[_pert_chain].first + _chains[_pert_chain].second; ++i )
        _npe[i] = (_npe[i] == V) ? H : V;
    } else if (_pert_type == 2) {                     // Operand-operator swap. It might require insertion/deletions in the list
      size_t op_pos = _chains[_pert_chain].first + ((!_pert_side) ? -1 : _chains[_pert_chain].second),
            opt_pos = _chains[_pert_chain].first + ((!_pert_side) ? 0 : _chains[_pert_chain].second-1);
      _chains[_pert_chain] = {_chains[_pert_chain].first+(!_pert_side), --_chains[_pert_chain].second};
      if (_npe[op_pos + (2*_pert_side - 1)] < 0){
        _chains[_pert_chain + (2*_pert_side - 1)].first -= _pert_side;
        ++_chains[_pert_chain + (2*_pert_side - 1)].second;
      } else if (_chains[_pert_chain].second == 0) _chains[_pert_chain] = {op_pos, 1};
      else _chains.insert(std::next(_chains.begin(), _pert_chain+_pert_side), {op_pos, 1});
      if(_chains[_pert_chain].second == 0) _chains.erase(std::next(_chains.begin(), _pert_chain));
      size_t ix=0;
      while(_operand_pos[ix] != op_pos) ++ix; // Find the operator
      _operand_pos[ix] = opt_pos;
      std::swap(_npe[op_pos], _npe[opt_pos]);
    } else {
      cerr << "Error [NPE]: Bad perturbation type" << endl;
      exit(EXIT_FAILURE);
    }
  } else {
      cerr << "Error [NPE]: Out of bounds while applying perturbation" << endl;
      exit(EXIT_FAILURE);
  }
  _pert_type = -1;    // Clear the stored perturbation
}

// Generate a random perturbation from the three possible choices
pair<size_t, size_t> NPE::gen_rnd_perturbation() {
  _pert={0,0};
  while(!_pert.first && !_pert.second) {
    _pert_type = rand()%3;
    switch(_pert_type){
      case 0: //swap two adjacent operands
        _pert = gen_rnd_operand_swap();
        break;
      case 1: // Invert a chain
        _pert = gen_rnd_chain_inversion();
        break;
      case 2: // Swap an operand-operator pair
        _pert = gen_rnd_operand_operator_swap();
        break;
    }
  }
  return _pert;
}

pair<size_t, size_t> NPE::gen_rnd_operand_swap() {
  size_t rand_op =rand()%(n_operands()-1);
  _pert_op1 = _operand_pos[rand_op];
  _pert_op2 = _operand_pos[rand_op+1];
  return { _operand_pos[rand_op], _operand_pos[rand_op+1] };
}

// Returns a pair of <index, 0> to indicate the chain swap
pair<size_t, size_t>  NPE::gen_rnd_chain_inversion() {
  size_t rand_chain = rand()% n_chains();
  _pert_chain = rand_chain;
  return { _chains[rand_chain].first, 0 };
}

pair<size_t, size_t> NPE::gen_rnd_operand_operator_swap() {
  size_t rnd_choice, rnd_chain, rnd_opt_pos, rnd_op_pos, num_ops;
  bool done = false, rnd_side;
  size_t attempts = 0;

  // Need to have a maximum number of tries as there are degenerate cases where this movement is not feasible.
  // Eg. 1-spaced, 1 length chains with the same separator (as the ones generated in the initial instance)
  // Check a multiple of n times before exiting
  while( !done && attempts < 10*_npe.size()) {
    // Choose a chain end at random
    rnd_choice  = rand()%((n_chains()<<1)-1),
    rnd_chain = rnd_choice>>1;
    rnd_side = rnd_choice%2;
    rnd_opt_pos = _chains[rnd_chain].first + rnd_side*(_chains[rnd_chain].second - 1),
    rnd_op_pos = rnd_opt_pos + (rnd_side<<1)-1;
    if (!rnd_side) {  // If an operand-operator pair is chosen
      num_ops=1;      // Count the number of operators (including the chosen one) for the balloting rule
      for(size_t i=0; i < rnd_chain; ++i ) num_ops += _chains[i].second;
    } else {          // If an operator-operand pair is chosen
      num_ops=0;      // If we're moving the operator to the right we will never break the balloting rule
    }
    if ( _npe[rnd_op_pos-1] != _npe[rnd_op_pos+1] && 2*num_ops < rnd_op_pos+1 ) done = true;
    ++attempts;
  }

  if ( !done ) return {0,0};
  _pert_chain = rnd_chain;
  _pert_side = rnd_side;
  return {rnd_op_pos, rnd_opt_pos};
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