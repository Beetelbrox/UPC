#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <stack>
#include <numeric>
#include <memory>
#include <chrono>

#include "floorplanning_solver.h"
#include "shape_function.h"


using namespace std;

Floorplanning_solver::Floorplanning_solver(const Floorplanning_problem &p):
  _problem(p),
  _slicing_tree(_problem.size()*2-1),
  _rng_engine(chrono::high_resolution_clock::now().time_since_epoch().count())
{
  generate_random_npe(_problem.size(), _npe, 1);
  build_slicing_tree();
}



void Floorplanning_solver::build_slicing_tree() {

}

int Floorplanning_solver::solve() {


  // QUESTIONS
  // 1. Solution Correctness
  // 2. Problem size
  // 3. Reasonable execution time
  // 4. Incremental wirelength
  // 

  /* Simmulated Annealing
    1.- Perturb the initial solution a number of times (number of operands?) to compute the average of all positive uphill climbes avg_uphill
    2.- Calculate the initial temperature as -avg_uphill/ln(P)



  */

  _npe.print();
  float initial_cost = calculate_cost();
  cerr << "Initial Cost: " << initial_cost << endl;
  pair<int, int> perturbation;

  float p = 0.99;

  float cost_cummulator = 0.0, delta_cost, cost_counter = 0.0;
  for(int i=0; i < 100; ++i) {
    perturbation = gen_rnd_perturbation();
    delta_cost = calculate_cost(perturbation) - initial_cost;
    cost_cummulator += abs(delta_cost);
  }
  float avg_uphill_cost = cost_cummulator/float(_npe.size());
  float initial_temp = -avg_uphill_cost / log(p);
  cerr << "Initial temp: " << initial_temp << endl;



  int k = 50000, reject = 0, its = 0, thr = 7, c = 100;
  float last_cost = initial_cost, cur_cost, temp = initial_temp, best_cost = initial_cost, error = 0.001;
  NPE best_npe;

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

  do {
    ++its;
    reject = 0;
    cost_cummulator = 0;
    cost_counter = 0;

    if (its > 1) k = 1000;

    for (int iteration=0; iteration < k; ++iteration) {
      perturbation = gen_rnd_perturbation();
      cur_cost = calculate_cost(perturbation);
      delta_cost = cur_cost - last_cost;
      if (delta_cost <= 0 || (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) < exp(-delta_cost/temp))) {
        _npe.apply_perturbation(perturbation);
        //_npe.print();
        if (cur_cost < best_cost) {
          best_cost = cur_cost;
          best_npe = NPE(_npe.get_npe_seq(), _npe.size());
          cerr << "Best Cost: " << best_cost << endl;
        }
      } else ++reject;
      cost_cummulator+= abs(delta_cost)/max(last_cost, cur_cost);
      ++cost_counter;
      last_cost = cur_cost;
    }
    if (its <= 1) _npe = NPE(best_npe.get_npe_seq(), _npe.size());
    else {
      temp = (initial_temp * (cost_cummulator/cost_counter))/its;
      if (its <= thr) temp = temp/c;
    }
    end = std::chrono::steady_clock::now();
  } while (reject/k <= 0.95 && temp >= error && std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() <= 300 );
  _npe = NPE(best_npe.get_npe_seq(), _npe.size());
  calculate_cost({-1,-1}, 1);

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

float Floorplanning_solver::calculate_cost( pair<int, int> perturbation, int print_sol ) {
  vector<Floorplanning_solution> solutions;
  pack_npe(solutions, perturbation);
  float best_cost = INFINITY;
  int best_sol = 0;
  for (size_t i=0; i < solutions.size(); ++i) {
    if (solutions[i].cost() < best_cost){
      best_cost = solutions[i].cost();
      best_sol = i;
      } 
  }
  if(print_sol) solutions[best_sol].print();
  return best_cost;
};

unique_ptr<Shape_function> Floorplanning_solver::pack_npe(int ix ) {
  if(_npe[ix] < 0 ) return unique_ptr(_problem.get_module_sf( _npe[ix] )
  else {
    return make_unique<Shape_function>(pack_npe())
  }
}

// The NPE object ensures correctness of the member NPE, so we don't have to check
void Floorplanning_solver::pack_npe( vector<Floorplanning_solution> &solutions, pair<int, int> perturbation ) {
  stack<Shape_function*> fp_stack;
  Shape_function *l_child, *r_child;
  unique_ptr<Shape_function> new_sf;

  int next_npe_element;
  bool inverted_chain = false;

  for( size_t i=0; i < _npe.size(); ++i ) {

    if ( _npe[i] > 0 ) {
      fp_stack.push( _problem.get_module_sf( next_npe_element ) ); // get_floorplan takes an id >= 1
    } 
    else {
      r_child = fp_stack.top(); fp_stack.pop(); // Remember that popping from a stack returns items in reverse insertion order
      l_child = fp_stack.top(); fp_stack.pop();
      fp_stack.push(make_unique<Shape_function>(_npe[i], ))
      merge_shape_functions(next_npe_element, l_child, r_child, fp_ptr);
      //fp_ptr->print();
      fp_stack.push(fp_ptr++);
    }
  }

  for(size_t i=0; i < fp_stack.top()->size(); ++i) {
    solutions.emplace_back(_npe.n_operands(), fp_stack.top(), i);
  }
}
/*

int Floorplanning_solver::merge_shape_functions(int op, const Floorplan* fp_1, const Floorplan* fp_2, Floorplan* fp_packed) {
  if(!fp_1->size() || !fp_2->size()) return -1; // If any of the floorplans is empty there's nothing to pack so return (malformed problem)
  // From now on we know that both floorplans have at least size 1
  vector<pair<int, int>> new_sf, subfp_ix;
  size_t ix_1=0, ix_2=0;
  
  if ( op == NPE::H ) {
    while( ix_1 < fp_1->size() || ix_2 < fp_2->size() )
      if ( fp_1->get_shape_width(ix_1) <= fp_2->get_shape_width(ix_2) ) {
        if (fp_1->get_shape_width(ix_1) > fp_2->get_shape_width(0) ){  // If this shape is eliminated by the algorithm, skip
          new_sf.emplace_back(fp_1->get_shape_width(ix_1), fp_1->get_shape_height(ix_1) + fp_2->get_shape_height(ix_2-1));
          subfp_ix.emplace_back(ix_1, ix_2-1);
        }
        ++ix_1;
      } else {
        if (fp_2->get_shape_width(ix_2) >= fp_1->get_shape_width(0) ){
          new_sf.emplace_back(fp_2->get_shape_width(ix_2), fp_1->get_shape_height(ix_1-1) + fp_2->get_shape_height(ix_2));
          subfp_ix.emplace_back(ix_1-1, ix_2);
        }
        ++ix_2;
      }
  } else if ( op == NPE::V ) {
    // Find out which floorplan has the largest h
    while(ix_1 < fp_1->size() && ix_2 < fp_2->size()) {
      if ( fp_1->get_shape_height(ix_1) >= fp_2->get_shape_height(ix_2) ) {
        new_sf.emplace_back(fp_1->get_shape_width(ix_1) + fp_2->get_shape_width(ix_2), fp_1->get_shape_height(ix_1));
        subfp_ix.emplace_back(ix_1, ix_2);
        if ( fp_1->get_shape_height(ix_1) == fp_2->get_shape_height(ix_2) ) ++ix_1;
        ++ix_1;
      } else {
        new_sf.emplace_back(fp_1->get_shape_width(ix_1) + fp_2->get_shape_width(ix_2), fp_2->get_shape_height(ix_2));
        subfp_ix.emplace_back(ix_1, ix_2);
        ++ix_2;
      }
    }
  } else cerr << "This state should not be reached" << endl; // Panic
  *fp_packed = Floorplan(op, new_sf,subfp_ix, fp_1, fp_2);
  return -!fp_packed->size();
  return 0;
}
*/

pair<int, int> Floorplanning_solver::gen_rnd_perturbation() {
  while(true) { // We will exit this loop by returning
    switch(rand()%3){
      case 0: //swap two adjacent operands
        return gen_rnd_operand_swap();
      case 1:
        return gen_rnd_chain_inversion();
      case 2:
        pair<int, int> swap = gen_rnd_operand_operator_swap();
        while(swap.first < 0) swap = gen_rnd_operand_operator_swap();
        return swap;
    }
  }
}

// Assumes correctness of the input
pair<int, int> Floorplanning_solver::gen_rnd_operand_swap() {
  int rand_operand = rand()%(_npe.n_operands()-1); // Choose a number UAR between the fist and the last
  return {_npe.get_operand_pos(rand_operand), _npe.get_operand_pos(rand_operand+1) };
}

// Returns a pair of <index, -1> to indicate the chain swap
pair<int, int>  Floorplanning_solver::gen_rnd_chain_inversion() {
  int rand_chain = rand()% _npe.n_chains();
  return { _npe.get_chain_pos(rand_chain), -1 };
}

// This assumes correctness on the input, as it is a private function. Will panic if it's not.
// ch_ix needs to take a value between 0 and length(chains) - 1. Side cant take either value 0 ir 1,
// only taking value 0 if ch_ix == length(chains) - 1.
pair<int, int> Floorplanning_solver::gen_rnd_operand_operator_swap() {

  // Choose a chain end at random excluding the last one so this is as uniform as possible
  int rnd_choice = rand()%(_npe.n_chains()<<1),
      rnd_chain = (rnd_choice)>>1,
      rnd_side = rnd_choice%2,
      rnd_operator_pos = _npe.get_chain_pos(rnd_chain) + rnd_side*(_npe.get_chain_length(rnd_chain) - 1),
      rnd_operand_pos = rnd_operator_pos + (rnd_side<<1) - 1;
  // If the swap breaks the skewness of the tree or if tries to swap with the tree root, panic and jump out
  if((_npe.get_element(rnd_operator_pos)) == _npe.get_element(rnd_operator_pos + (rnd_side<<2) - 2) || (rnd_operator_pos == int(_npe.size()-1))) return {-1,-1};

  // Check that the perturbation satisfies the balloting rule (only needs to be done if
  // we are pushing operators back
  if (!rnd_side) {
    int op_counter = 1;
    for (size_t i=0; i < rnd_chain; ++i) op_counter += _npe.get_chain_length(i);
    if ( 2*op_counter >= rnd_operand_pos+1 ) return {-1,-1};
  }
  // If the perturbation is valid, swap the values in the vector and return 0
  return {rnd_operator_pos, rnd_operand_pos};
}

