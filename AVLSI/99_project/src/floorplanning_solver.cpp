#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <stack>
#include <numeric>

#include "floorplanning_solver.h"
#include "floorplan.h"

using namespace std;

Floorplanning_solver::Floorplanning_solver(const Floorplanning_problem &p): _problem(p) {
  generate_random_npe(100, _current_npe, 1);
  pack_npe();
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
  //pack_npe();
  //calculate_floorplan_dimensions(solutions.back().get_npe());
  return 0;
}

//  std::mt19937{std::random_device{}()} For the RNG

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


// The NPE object ensures correctness of the member NPE, so we don't have to check
pair<int, int> Floorplanning_solver::pack_npe() {
  stack<const Floorplan *> fp_stack;
  const Floorplan *fp_1, *fp_2;
  for(int* npe_it = _current_npe.begin(); npe_it != _current_npe.end(); ++npe_it) {
    if ( *npe_it > 0) fp_stack.push(_problem.get_floorplan(npe_it-_current_npe.begin() + 1 ) ); // get_floorplan takes an id >= 1
    else {
      Floorplan fp_packed;

      fp_1 = fp_stack.top(); fp_stack.pop();
      fp_2 = fp_stack.top(); fp_stack.pop();
      pack_floorplans(*npe_it, fp_1, fp_2, fp_packed);

    }
  }
  return {0,0};
}

void Floorplanning_solver::pack_floorplans(int op, const Floorplan* fp_1, const Floorplan* fp_2, Floorplan &fp_packed) {
  const Floorplan *first_fp, *second_fp;
  // Find out which floorplan has the smallest w
  if ( fp_1->get_shape(0)->first <= fp_2->get_shape(0)->first) {
    first_fp = fp_1;
    second_fp = fp_2;
  } else {
    first_fp = fp_2;
    second_fp = fp_1;
  } // 0 for the first one

  pair<int, int>* it_1 = first_fp->begin(); , it_2 = second_fp->begin();;

  for (pair<int, int>* f_it = first_fp->begin(); f_it != first_fp->end(); ++f_it) {
    if ( op == NPE::H ) {

    }
    cout << f_it->first << " " <<  second_fp->begin()->first << endl;
  }



  } else if ( op == NPE::V ) {

  } else { // Panic
    cerr << "This state should not be reached" << endl;
  }
}

/*
pair <int, int> Floorplanning_solver::calculate_floorplan_dimensions(const vector<int> &npe) {

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
*/
