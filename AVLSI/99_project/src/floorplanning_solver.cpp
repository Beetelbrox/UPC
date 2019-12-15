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
  _npe_mapping(_npe.size()),
  _rng_engine(std::chrono::high_resolution_clock::now().time_since_epoch().count())
{
  _npe.print();
  _slicing_tree_root = build_slicing_tree(_npe, _slicing_tree);
  cerr << "Optimal area: " << _slicing_tree_root->get_min_area() << endl;
}

Shape_function* Floorplanning_solver::build_slicing_tree(NPE &npe, vector<Shape_function> &sl_tree) {
  std::stack<Shape_function*> sf_stack;
  Shape_function *l_child, *r_child;
  int e, op_cont = 0;
  
  for(size_t i=0; i <npe.size(); ++i) {
    e = npe.get_pert_element(i);
    if( e > 0 )  {
      sl_tree[e-1] = Shape_function(e, *_problem.get_module_sf_seq(e));
      sf_stack.push(&sl_tree[e-1]);
    } else if (e == NPE::V || e == NPE::H) {
      
      r_child = sf_stack.top(); sf_stack.pop();
      l_child = sf_stack.top(); sf_stack.pop();
      sl_tree[_problem.size()+op_cont] = Shape_function(e, l_child, r_child);
      
      l_child->set_parent(&sl_tree[_problem.size()+op_cont]);
      r_child->set_parent(&sl_tree[_problem.size()+op_cont]);
      sf_stack.push(&sl_tree[_problem.size()+op_cont]);
      ++op_cont;
    } else {
      cerr << "Error [NPE]: Malformed NPE sequence while building slicing tree" << endl;
      exit(EXIT_FAILURE);
    }
  }
  return sf_stack.top();
}

void Floorplanning_solver::solve() {


  // QUESTIONS
  // 1. Solution Correctness
  // 2. Problem size
  // 3. Reasonable execution time
  // 4. Incremental wirelength
  // 
// Simmulated Annealing
//    1.- Perturb the initial solution a number of times (number of operands?) to compute the average of all positive uphill climbes avg_uphill
//    2.- Calculate the initial temperature as -avg_uphill/ln(P)

  vector<Shape_function> tentative_tree = vector<Shape_function>(_npe.size());
  Shape_function* tentative_tree_root;
  pair<size_t, size_t> perturbation;
  Floorplanning_solution tentative_solution, best_solution;
  pair <float, Floorplanning_solution> sol, best_sol;

  float last_cost,
    avg_cost = 0.0,
    delta_cost = 0.0,
    initial_temp,
    temp,
    p = 0.99,
    r = 0.85,
    error = 0.001;
  
  int k=5, reject, initial_pert_number = _npe.size()*10;

  best_sol = get_best_solution(_slicing_tree_root);
  cerr << "Initial Cost: " << best_sol.first << endl;
  last_cost = best_sol.first;

  _npe.print();
  for(int i=0; i < initial_pert_number; ++i) {

    _npe.gen_rnd_perturbation(); 
    _npe.apply_perturbation();
    _npe.print();
    
    tentative_tree_root = build_slicing_tree(_npe, tentative_tree);  // Build the slicing tree

    sol = get_best_solution(tentative_tree_root);
    delta_cost = sol.first - last_cost;
    avg_cost += abs(delta_cost);
  }
  avg_cost = avg_cost/float(initial_pert_number);
  initial_temp = -avg_cost/log(p);
  temp = initial_temp;
  cerr << "Initial temp: " << initial_temp << endl;

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  
  do {
    reject = 0;
    for (int i=0; i < k; ++i) {
      _npe.gen_rnd_perturbation();
      tentative_tree_root = build_slicing_tree(_npe, tentative_tree);  // Build the slicing tree
      sol = get_best_solution(tentative_tree_root);
      delta_cost = sol.first - last_cost;
      if (delta_cost <= 0 || (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) < exp(-delta_cost/temp))) {
        _npe.apply_perturbation();
        last_cost = sol.first;
        if (sol.first < best_sol.first) best_sol = sol;
      } else ++reject;
    }
    temp =  temp*r; // Reduce temperature

  } while (reject/k <= 0.95 && temp >= error && std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() <= 180 );
  cerr << "Cost: " << best_sol.first << endl;
  best_sol.second.print();
}

pair<float, Floorplanning_solution> Floorplanning_solver::get_best_solution(Shape_function *slicing_tree_root) {
  Floorplanning_solution sol, best_sol;
  float best_cost = INFINITY;
  for (size_t i=0; i<slicing_tree_root->size(); ++i) {
    sol = Floorplanning_solution(slicing_tree_root, _problem.size(), i);
    if (sol.cost() < best_cost) {
      best_cost = sol.cost();
      best_sol = sol;
    }
  }
  return {best_cost, best_sol};
}

