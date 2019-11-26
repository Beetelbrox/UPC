#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <stack>
#include "floorplanning_solver.h"
#include "floorplan.h"

using namespace std;

Floorplanning_solver::Floorplanning_solver(const Floorplanning_problem &p): problem(p) {
  vector<int> npe_vec;

  generate_random_npe(p.size(), npe_vec, true);
  npe = make_unique<NPE>(npe_vec);
  npe->print();
}

int Floorplanning_solver::solve() {
  /*
  for(int i=0; i <1000; ++i) {
    pair<int, int> perturbation = npe->gen_rnd_perturbation();
    //std::cout << "(" << perturbation.first << "," << perturbation.second << ")" << std::endl;
    npe->apply_perturbation(perturbation);
    //npe->print();
  }
  */
  pack_npe();
  //calculate_floorplan_dimensions(solutions.back().get_npe());
  return 0;
}

// Refactor this to make it random without the weird constraints
void Floorplanning_solver::generate_random_npe(size_t size, vector<int> &npe_vec, bool shuffle) {
  cerr << "Generating initial solution...";
  size_t n_operators=0, n_operands=0, next_insert;
  int operands[size];
  npe_vec.resize(2*size-1);
  // Create a sequence of indices and shuffle it if requested
  for (size_t i=0; i<size; ++i) operands[i] = i+1;
  if(shuffle) random_shuffle(&operands[0], &operands[size]);

  while(n_operators+1 < size) {
    if (n_operands >= size) next_insert = 1;       // All operands inserted
    else if (n_operators+1 >= n_operands) next_insert = 0;  // Balloting rule
    else next_insert = rand()%2;

    // 1 insert operator, 0 insert operand
    if(next_insert) {
      npe_vec[n_operands + n_operators] = npe_vec[n_operands+n_operators-1] < 0 ? !(npe_vec[n_operands+n_operators-1] + 2) - 2 : (rand()%2)-2;
      ++n_operators;
    } else {
      npe_vec[n_operands + n_operators] = operands[n_operands];
      ++n_operands;
    }
  }
  cerr << "done." << endl;
}

pair<int, int> Floorplanning_solver::pack_npe() {
  stack<Floorplan *> fp_stack;
  for(size_t i=0; i < npe->size(); ++i) {
    //if(npe->get_element(i) > 0) fp_stack.push(problem.get_module_floorplan(npe->get_element(i)));
  }
}

pair <int, int> Floorplanning_solver::calculate_floorplan_dimensions(const vector<int> &npe) {
/*
  stack<vector<pair<int, int>>> module_stack;
  vector<pair<int, int>> shape_function, submod_1, submod_2;
  for(int e : npe) {
    vector<pair<int, int>> shape_function;
    if(e > 0) {
      // Generate the shape function and push it into the queue
      for (pair <int, int> p : problem.get_module_dimensions(e-1)) {
        shape_function.push_back(p);
        if(p.first != p.second) shape_function.push_back({p.second, p.first});
      }
      module_stack.push(problem.get_module_dimensions(e-1));
    } else if (e < 0) {
      cout << e << endl;
      submod_1 = module_stack.top(); module_stack.pop();
      submod_2 = module_stack.top(); module_stack.pop();
    } else {
      cerr << "Error: Bad index in NPE." << endl;
      return {-1,-1};
    }
  }
  */
  return {0,0};
}


// If parse=false, it DOES NOT CHECK CORRECTNESS


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
