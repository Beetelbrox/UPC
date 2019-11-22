#include <iostream>
#include <cstdlib>
#include <utility>
#include "npe.h"

using namespace std;

NPE::NPE(const vector<int> &seq): npe(seq) {
  bool is_chain = false;
  for(size_t i=0; i < seq.size(); ++i) {
    if (npe[i] > 0 && npe[i] <= int((seq.size()+1)/2)) { // Cast tp avpod a warmning
      operand_pos.push_back(i);
      if (is_chain) is_chain = false;
    } else if (npe[i] == V || npe[i] == H){
      if( is_chain ) prev(chains.end(),1)->second++;
      else {
        chains[i] = 1;
        is_chain = true;
      }
    } else {
      cerr << "Error: Malformed input sequence" << endl;
      operand_pos.clear();
      chains.clear();
      npe.clear();
    }
  }
}

vector<int> NPE::generate_random_perturbation() {
  vector<int> perturbation = npe; // Copy the npe
  while(true) { // We will exit this loop by returning
    switch(rand()%3){
      case 0: //swap two adjacent operands
        swap_operands(perturbation, rand()%operand_pos.size()-1);
        return perturbation;
        break;
      case 1:
        invert_chain(perturbation, rand()%chains.size());
        return perturbation;
        break;
      case 2:
        int rnd_pick = rand()%(2*chains.size());
        if(!swap_operand_operator(perturbation, rnd_pick >> 1, rnd_pick%2))
          return perturbation;
        break;
    }
  }
}

// Assumes correctness of the input
void NPE::swap_operands(vector<int> &perturbation, size_t ix) {
  swap(perturbation[operand_pos.at(ix)], perturbation[operand_pos.at(ix+1)]);
}

// Assumes correctness of the input
void NPE::invert_chain(vector<int> &perturbation, size_t ch_ix) {
  map<int, int>::iterator chains_it = next(chains.begin(), ch_ix);
  for(int i=0; i < chains_it->second; ++i ) {
    perturbation[chains_it->first + i] = (perturbation[chains_it->first + i] == V) ? H : V;
  }
}

// This assumes correctness on the input, as it is a private function. Will panic if it's not.
// ch_ix needs to take a value between 0 and length(chains) - 1. Side cant take either value 0 ir 1,
// only taking value 0 if ch_ix == length(chains) - 1.
int NPE::swap_operand_operator(vector<int> &perturbation, size_t ch_ix, bool side) {
  map<int, int>::iterator chain_head_it = next(chains.begin(), ch_ix);
  int operator_pos = chain_head_it->first + side*(chain_head_it->second-1),
      operand_pos = operator_pos + (2*side-1);

  // Check that the perturbation generates a skewed tree
  if (npe.at(operator_pos) == npe.at(operator_pos + 2*(2*side-1))) {
    cerr << "Invalid perturbation: Not skewed tree" <<  endl;
    return -1;
  }

  // Check that the perturbation satisfies the balloting rule (only needs to be done if
  // we are pushing operators back
  if (!side) {
    int num_operators = 1;
    for (map<int, int>::iterator it = chains.begin(); it != next(chains.begin(), ch_ix); ++it ) num_operators += it->second;
    if ( 2*num_operators >= operand_pos ) {
      cerr << "Invalid perturbation: Balloting rule broken" << endl;
      return -1;
    }
  }
  // If the perturbation is valid, swap the values in the vector and return 0
  swap( perturbation[operator_pos], perturbation[operand_pos] );
  return 0;
}

void NPE::print() {
  for (int &e : npe) {
    if (e == V) cout << "V";
    else if (e == H) cout << "H";
    else cout << e;
    cout << " ";
  }
  cout << endl;
}
