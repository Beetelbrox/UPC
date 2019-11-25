#include <iostream>
#include <cstdlib>
#include <utility>
#include <unordered_set>
#include <iterator>
#include "npe.h"

using namespace std;

NPE::NPE(const vector<int> &seq): length{seq.size()}, num_operands{(length+1)>>1}, num_operators{length>>1} {
  // Allocate space with smart pointers for extra convenience.
  npe = make_unique<int[]>(length);
  for(size_t i=0; i <length; ++i) npe[i] = seq[i];  // Copy the whole vector into the member var. If it is malformed it will panic anyways so its' not a great waste
  if(parse_npe()) length = 0; // If the parsing fails due to malformation, set the length to 0;
}

int NPE::parse_npe(){
  // Clean the index data structures
  operand_pos = make_unique<int[]>(num_operands);
  chains.clear();
  size_t operand_counter = 0, operator_counter = 0;
  unordered_set <size_t> read_operands; // Hash table to check for already inserted elements

  for(size_t i=0; i < length; ++i) {
    // Check that the ID is within bounds (As it will be used for indexing on the floorplans) and that we are not repeating elements
    if (npe[i] > 0 && npe[i] <= int(num_operands) && read_operands.find(npe[i]) == read_operands.end()) {
      if (operand_counter >= num_operands) break; // Check for excesss in the number of operands
      operand_pos[operand_counter++] = i; // This should not go out of bounds as we're checking correctness before
      read_operands.insert(npe[i]);
    } else if ( (npe[i] == V || npe[i] == H) && (operator_counter + 1 < operand_counter) ) { // Check that the operator IDs are correct and that it meets the balloting rule before allowing to insert operators
      if(operator_counter >= num_operators) break; // Check for exess on the number of operators
      if (npe[i-1] > 0) chains.push_back({i, 1}); // If is a new chain, add it to the list. Relies on correctness of previous elements (it should have freaked out by now)
      else if (npe[i-1] != npe[i]) ++chains.back().second; // If the current element is part of a chain and is a skewed tree, add it to the chain.
      else break; // If the tree is not skewed, panic
      ++operator_counter;
    } else break; // If we reach this state, the input is malformed
  }
  // If the number of operands and operators is not the expected, panic and release the pointer resources
  if (operand_counter < num_operands || operator_counter < num_operators) {
    cerr << "Error: Malformed npe sequence" << endl;
    num_operands = num_operators = 0;  // Puts the length at 0. Data structures are handled by the default destructor (thanks smartpointers)
    operand_pos.reset(nullptr);
    chains.clear();
    return -1;
  }
  return 0;
}

// If parse=false, it DOES NOT CHECK CORRECTNESS
int NPE::apply_perturbation(const pair<int, int> &perturbation, bool parse) {
  if (perturbation.second >= 0) swap(npe[perturbation.first], npe[perturbation.second]);
  else {
    for(size_t i=0; perturbation.first + i < length; ++i) {

      if (npe[perturbation.first + i] >= 0) break;
      npe[perturbation.first+i] = (npe[perturbation.first+i] == V) ? H : V;
    }
  }
  if(!parse) return 0;
  if(parse_npe()) {
    apply_perturbation(perturbation, false);
    return -1;
  }
  return 0;
}

//TODO do we need an assignment operator?

pair<int, int> NPE::gen_rnd_perturbation() {
  while(true) { // We will exit this loop by returning
    switch(rand()%3){
      case 0: //swap two adjacent operands
        return gen_rnd_operand_swap();
      case 1:
        return gen_rnd_chain_swap();
      case 2:
        pair<int, int> swap = gen_rnd_operand_operator_swap();
        if (swap.first >= 0) return swap;
    }
  }
}

// Assumes correctness of the input
pair<int, int> NPE::gen_rnd_operand_swap() {
  int rand_operand = rand()%(num_operands-1); // Choose a number UAR between the fist and the last
  return {operand_pos[rand_operand], operand_pos[rand_operand+1]};
}

// Returns a pair of <index, -1> to indicate the chain swap
pair<int, int>  NPE::gen_rnd_chain_swap() {
  int rand_chain = rand()%chains.size();
  return {chains[rand_chain].first, -1};
}

// This assumes correctness on the input, as it is a private function. Will panic if it's not.
// ch_ix needs to take a value between 0 and length(chains) - 1. Side cant take either value 0 ir 1,
// only taking value 0 if ch_ix == length(chains) - 1.
pair<int, int> NPE::gen_rnd_operand_operator_swap() {
  // Choose a chain end at random excluding the last one so this is as uniform as possible
  int rnd_choice = rand()%(chains.size()<<1),
      rnd_chain = (rnd_choice)>>1,
      rnd_side = rnd_choice%2,
      rnd_operator_pos = chains[rnd_chain].first + rnd_side*(chains[rnd_chain].second - 1),
      rnd_operand_pos = rnd_operator_pos + (rnd_side<<1) - 1;
  // If the swap breaks the skewness of the tree or if tries to swap with the tree root, panic and jump out
  if((npe[rnd_operator_pos] == npe[rnd_operator_pos + (rnd_side<<2) - 2]) || (rnd_operator_pos == int(length)-1)) return {-1,-1};

  // Check that the perturbation satisfies the balloting rule (only needs to be done if
  // we are pushing operators back
  if (!rnd_side) {
    int op_counter = 1;
    for (vector<pair<int, int>>::iterator it = chains.begin(); it != next(chains.begin(), rnd_chain); ++it ) op_counter += it->second;
    if ( 2*op_counter >= rnd_operand_pos+1 ) return {-1,-1};
  }
  // If the perturbation is valid, swap the values in the vector and return 0
  return {rnd_operator_pos, rnd_operand_pos};
}

size_t NPE::get_length() { return length; }
size_t NPE::get_num_operands() { return num_operands; }
size_t NPE::get_num_operators() { return num_operators; } // This is an integer division

void NPE::print() {
  for (size_t i=0; i<length; ++i) {
    if (npe[i] == V) cout << "V";
    else if (npe[i] == H) cout << "H";
    else cout << npe[i];
  }
  cout << endl;
}
