#include <iostream>
#include <iterator>
#include "floorplanning_solution.h"


using namespace std;


Floorplanning_solution::Floorplanning_solution(const std::vector<int> &npe) {
  int current_chain;
  for(size_t i=0; i<npe.size(); ++i) {
    if (npe[i] > 0) operands[i] = npe[i];
    else if (npe[i] < 0){
      if (npe[i-1] > 0) { // If the npe is well formed this will never go out of bounds as the two first elements must be operands
        current_chain = i;
        chains[current_chain] = vector<int>();
      }
      chains[current_chain].push_back(npe[i]);
    }
  }
}

void Floorplanning_solution::swap_operands(int ix) {
  map<int, int>::iterator operands_it = next(operands.begin(),ix);
  int aux = operands_it->second;
  operands_it->second = next(operands_it,1)->second;
  next(operands_it,1)->second = aux;
}

void Floorplanning_solution::invert_chain(int ix) {
  map<int, vector<int> >::iterator chains_it = next(chains.begin(),ix);
  for (int &op: chains_it->second) {
    op = ((op+3)%2)-2;
  }
}

int Floorplanning_solution::swap_operand_operator(int ix) {
  map<int, int>::iterator operand_it = next(operands.begin(),ix);
  map<int, vector<int> >::iterator chains_it
  int side;
  bool r_operator = operand_it->first+1 != next(operand_it, 1)->first; // Check if the element to the right is an operator by checking if the next element in the operand list is not adjacent
  bool l_operator = operand_it->first-1 != prev(operand_it, 1)->first;
  if (r_operator && l_operator ) side = rand()%2;
  else side = r_operator ? 1 : 0;
    chains_it = chains.find(operand_it->first+1);
  if (side) {

  } else {

  }
  } else { // We can get to the beginning of the previous chain by referencing from the next one (as there can only be 2 adjacent operators)
    if (operators.find(operand_it->first-2)
    chains_it = prev(chains.find(operand_it->first+2), 1);
    cout <<"op: " <<chains_it->first << endl;
  }
  // Try the next operator first. This assumes that there will always be at least one operand afterwards

  return 0;
}

int Floorplanning_solution::get_width(){ return width; }
int Floorplanning_solution::get_height(){ return height; }
int Floorplanning_solution::get_num_chains(){ return chains.size(); }

void Floorplanning_solution::print_npe() {
  map<int, int>::iterator operands_it;
  map<int, vector<int> >::iterator chains_it;
  for(size_t i=0; i < 2*operands.size()-1; ++i) {
    operands_it = operands.find(i);
    if (operands_it != operands.end()) {
      cerr << operands_it->second << " ";
    } else {  // If it's not in operands it must be in chains
      chains_it = chains.find(i);
      for (int op : chains_it->second) cerr << op << " ";
      i += chains_it->second.size()-1;
    }
  }
  cerr << endl;
}
