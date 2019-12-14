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


using std::cerr;
using std::endl;
using std::vector;
using std::pair;

Floorplanning_solver::Floorplanning_solver(const Floorplanning_problem &p):
  _problem(p),
  _npe(p.size(), 0),
  _slicing_tree(_npe.size()),
  _rng_engine(std::chrono::high_resolution_clock::now().time_since_epoch().count())
{
  _npe.print();
  build_slicing_tree();
}

void Floorplanning_solver::build_slicing_tree() {
  std::stack<Shape_function*> sf_stack;
  Shape_function *l_child, *r_child;
  int op_cont = 0;
  for(size_t i=0; i <_npe.size(); ++i) {
    if(_npe[i] > 0 )  {
      _slicing_tree[_npe[i]-1] = Shape_function(_npe[i], *_problem.get_module_sf_seq(_npe[i]));
      sf_stack.push(&_slicing_tree[_npe[i]-1]);
    } else if (_npe[i] == NPE::V || _npe[i] == NPE::H) {
      r_child = sf_stack.top(); sf_stack.pop();
      l_child = sf_stack.top(); sf_stack.pop();
      _slicing_tree[_problem.size()+op_cont] = Shape_function(_npe[i], l_child, r_child);
      sf_stack.push(&_slicing_tree[_problem.size()+op_cont]);
      ++op_cont;
    } else {
      cerr << "Error [NPE]: Malformed NPE sequence while building slicing tree" << endl;
      exit(EXIT_FAILURE);
    }
  }
  _slicing_tree_root = sf_stack.top();
  cerr << "Optimum area: " << _slicing_tree_root->get_min_area() << endl;
}

void Floorplanning_solver::solve() {
  std::vector<Floorplanning_solution> current_solutions;
  for( size_t i=0; i < _slicing_tree_root->size(); ++i ) {
    current_solutions.emplace_back(_slicing_tree_root, _problem.size(), i);
  }

  // Simmulated Annealing parameters
  double p=0.99;
  pair<size_t, size_t> perturbation;
  for(int i=0; i<2000; ++i){
    perturbation = gen_rnd_perturbation();
    cerr << "Perturbation: " << perturbation.first << " " << perturbation.second << endl;
    _npe.apply_perturbation(perturbation);
    _npe.print(true);
  }
}

/*

int Floorplanning_solver::solve() {


  // QUESTIONS
  // 1. Solution Correctness
  // 2. Problem size
  // 3. Reasonable execution time
  // 4. Incremental wirelength
  // 
 Simmulated Annealing
    1.- Perturb the initial solution a number of times (number of operands?) to compute the average of all positive uphill climbes avg_uphill
    2.- Calculate the initial temperature as -avg_uphill/ln(P)



 

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

*/

pair<size_t, size_t> Floorplanning_solver::gen_rnd_perturbation() {
  while(true) { // We will exit this loop by returning
    int rnd = rand()%3;
    switch(rnd){
      case 0: //swap two adjacent operands
        return gen_rnd_operand_swap();
      case 1:
        return gen_rnd_chain_inversion();
      case 2:
        return gen_rnd_operand_operator_swap();
    }
  }
}

// Assumes correctness of the input
pair<size_t, size_t> Floorplanning_solver::gen_rnd_operand_swap() {
  size_t rand_operand = rand()%(_npe.n_operands()-1); // Choose a number UAR between the fist and the last
  return {_npe.get_operand_pos(rand_operand), _npe.get_operand_pos(rand_operand+1) };
}

// Returns a pair of <index, -1> to indicate the chain swap
pair<size_t, size_t>  Floorplanning_solver::gen_rnd_chain_inversion() {
  size_t rand_chain = rand()% _npe.n_chains();
  return { _npe.get_chain_pos(rand_chain), 0 };
}

// This assumes correctness on the input, as it is a private function. Will panic if it's not.
// ch_ix needs to take a value between 0 and length(chains) - 1. Side cant take either value 0 ir 1,
// only taking value 0 if ch_ix == length(chains) - 1.
pair<size_t, size_t> Floorplanning_solver::gen_rnd_operand_operator_swap() {
  size_t rnd_choice, rnd_chain, rnd_opt_pos, rnd_op_pos, num_ops;
  bool done = false, rnd_side;

  while( !done ) {
    // Choose a chain end at random
    rnd_choice  = rand()%((_npe.n_chains()<<1)-1),
    rnd_chain = (rnd_choice)>>1;
    rnd_side = rnd_choice%2;
    rnd_opt_pos = _npe.get_chain_pos(rnd_chain) + rnd_side*(_npe.get_chain_length(rnd_chain) - 1),
    rnd_op_pos = rnd_opt_pos + (rnd_side<<1)-1;
    num_ops = 0;  // The selected operator is included in the count
    //cerr << rnd_choice << " " << rnd_side << " " << rnd_chain << " " << rnd_opt_pos << " " << rnd_op_pos << endl;    
    // Count the number of operators before the previously chosen chain
    for(size_t i=0; i < rnd_chain+ (!rnd_side); ++i ) num_ops += _npe.get_chain_length(i);
    if ( _npe[rnd_op_pos-1] != _npe[rnd_op_pos+1] && 2*num_ops < rnd_op_pos ) done = true;
  }
  return {rnd_op_pos, rnd_opt_pos};
}

/*
// This assumes correctness on the input, as it is a private function. Will panic if it's not.
// ch_ix needs to take a value between 0 and length(chains) - 1. Side cant take either value 0 ir 1,
// only taking value 0 if ch_ix == length(chains) - 1.
pair<int, int> Floorplanning_solver::gen_rnd_operand_operator_swap() {

  // Choose a chain end at random excluding the last one so this is as uniform as possible
  size_t rnd_choice = rand()%(_npe.n_chains()<<1),
      rnd_chain = (rnd_choice)>>1,
      rnd_side = rnd_choice%2,
      rnd_operator_pos = _npe.get_chain_pos(rnd_chain) + rnd_side*(_npe.get_chain_length(rnd_chain) - 1),
      rnd_operand_pos = rnd_operator_pos + (rnd_side<<1) - 1;
  // If the swap breaks the skewness of the tree or if tries to swap with the tree root, panic and jump out
  if((_npe[rnd_operator_pos]) == _npe[rnd_operator_pos + (rnd_side<<2) - 2] || (rnd_operator_pos == int(_npe.size()-1))) return {-1,-1};

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
*/

