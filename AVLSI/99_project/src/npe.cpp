#include <iostream>
#include "npe.h"

using namespace std;

NPE::NPE(const vector<int> &seq): npe(seq) {
  int current_chain;
  for(size_t i=0; i < seq.size(); ++i) {
    if (npe[i] > 0) operand_positions.push_back(i);
    else if (npe[i] < 0){
      //if (npe[i-1] > 0) { // If the npe is well formed this will never go out of bounds as the two first elements must be operands
      //  current_chain = i;
      //  chains[current_chain] = vector<int>();
      //}
      //chains[current_chain].push_back(npe[i]);
    } else {
      cerr << "Error: Malformed input sequence" << endl;
      operand_positions.clear();
      chain_positions.clear();
      npe.clear();
    }
  }
}
