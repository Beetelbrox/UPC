#include <iostream>
#include <cstdlib>
#include "npe.h"

using namespace std;

NPE::NPE(const vector<int> &seq): npe(seq) {
  bool is_chain = false;
  for(size_t i=0; i < seq.size(); ++i) {
    if (npe[i] > 0 && npe[i] <= int((seq.size()+1)/2)) { // Cast tp avpod a warmning
      operand_pos.push_back(i);
      if (is_chain) is_chain = false;
    } else if (npe[i] == V || npe[i] == H){
      if( is_chain ) prev(chain_pos.end(),1)->second++;
      else {
        chain_pos[i] = 1;
        is_chain = true;
      }
    } else {
      cerr << "Error: Malformed input sequence" << endl;
      operand_pos.clear();
      chain_pos.clear();
      npe.clear();
    }
  }

  print_npe();
  swap_operand_operator(1+rand()%(operand_pos.size()-2));
  print_npe();
  swap_operand_operator(1+rand()%(operand_pos.size()-2));
  print_npe();

}

vector<int> NPE::generate_random_permutation() {
  return vector<int>();
}

void NPE::swap_operands(size_t ix) {
  if (ix >= operand_pos.size()-1) cerr << "Error: Index out of bounds while trying to swap operands" << endl;
  else {
    int aux = npe[operand_pos[ix]];
    npe[operand_pos[ix]] = npe[operand_pos[ix+1]];
    npe[operand_pos[ix+1]] = aux;
  }
}

void NPE::invert_chain(size_t ix) {
  if (ix >= chain_pos.size()) cerr << "Error: Index out of bounds while trying to invert chains" << endl;
  else {
    map<int, int>::iterator chains_it = next(chain_pos.begin(),ix);
    for(int i=0; i < chains_it->second; ++i ) {
      npe[chains_it->first + i] = (npe[chains_it->first + i] == V) ? H : V;
    }
  }
}

int NPE::swap_operand_operator(size_t ix) {
  if (ix >= operand_pos.size()) cerr << "Error: Index out of bounds while trying to operand-operator" << endl;
  else if (ix <= 0) cerr << "Error: Index for operand-operator swap must be greater than 0" << endl;
  else {
    bool operand_at_left = operand_pos[ix-1] == operand_pos[ix]-1,
        operand_at_right = ix < operand_pos.size()-1 ? operand_pos[ix+1] == operand_pos[ix]+1 : true;
    if ( operand_at_left && operand_at_right ? rand()%2 : operand_at_left ) {

    }

  }
  return 0;
}

void NPE::print_npe() {
  for (int &e : npe) {
    if (e == V) cout << "V";
    else if (e == H) cout << "H";
    else cout << e;
    cout << " ";
  }
  cout << endl;
}
