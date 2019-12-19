/*########################################
 - File: floorplanning_solver.cpp
 - Author: Francisco Javier Jurado Moreno
 - Project: AVLSI Floorplanning project
##########################################*/

#include <iostream>
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
  _npe(p.size()),
  _slicing_tree(_npe.size())
{
  _slicing_tree_root = build_slicing_tree(_npe, _slicing_tree);
  cerr << "Optimal area: " << _slicing_tree_root->get_min_area() << endl;
}

// Builds a slicing tree from a giving NPE and it applies the stored perturbation (if any)
// It uses a stack in order to build the tree as when evaluating expressions. It assumes that the left child is placed to the left of the right side for vertical cuts 
// and that the left child is placed above the right child for horizontal cuts.
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


// Implements the (classic) simmulated annealing algorithm
void Floorplanning_solver::solve() {

  vector<Shape_function> tentative_tree = vector<Shape_function>(_npe.size());
  Shape_function* tentative_tree_root;
  pair<size_t, size_t> perturbation;
  pair <float, Floorplanning_solution> sol, best_sol;

  float last_cost,
    avg_cost = 0.0,
    delta_cost = 0.0,
    initial_temp,
    temp,
    // Simulated annealing parameters
    p = 0.99,
    r = 0.99,
    min_temp = 0.0001;
  
  // The number of iterations per temperature is 5 times the size of the npe, the initial number of pertubations to calculate the initial
  // temperature is 10 times the npe size
  int k=5*_npe.size(), reject, initial_pert_number = _npe.size()*10, ctr=0;

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  best_sol = get_best_solution(_slicing_tree_root);
  cerr << "Initial Cost: " << best_sol.first << endl;
  cerr << "Initial Area: " << best_sol.second.get_area() << endl;
  cerr << "Initial Wirelength: " << best_sol.second.get_wirelength() << endl;
  last_cost = best_sol.first;

  // Perturb the npe a number of times and record the uphill moves to calculate the initial temperature
  for(int i=0; i < initial_pert_number; ++i) {
    _npe.gen_rnd_perturbation(); 
    _npe.apply_perturbation();
    tentative_tree_root = build_slicing_tree(_npe, tentative_tree);
    sol = get_best_solution(tentative_tree_root);
    delta_cost = sol.first - last_cost;
    if ( delta_cost > 0) {
      avg_cost += delta_cost;
      ++ctr;
    }
  }
  avg_cost = avg_cost/ctr;
  initial_temp = -avg_cost/log(p);
  temp = initial_temp;
  cerr << "Initial temp: " << initial_temp << endl; 
  
  // Simmulated annealing algorithm as in the slides
  do {
    reject = 0;
    for (int i=0; i < k; ++i) {
      _npe.gen_rnd_perturbation();
      tentative_tree_root = build_slicing_tree(_npe, tentative_tree);
      sol = get_best_solution(tentative_tree_root);
      delta_cost = sol.first - last_cost;
      if (delta_cost <= 0 || (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) < exp(-delta_cost/temp))) {
        _npe.apply_perturbation();
        last_cost = sol.first;
        if (sol.first < best_sol.first) {
          best_sol = sol;
          cerr << "Best cost: " << best_sol.first << endl;
        }
      } else ++reject;
    }
    temp =  temp*r; // Reduce temperature

  } while (reject/k <= 0.95 && temp >= min_temp);
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  cerr << "Elapsed time: " <<  std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << " seconds" << endl;
  cerr << "Best Cost: " << best_sol.first << endl;
  cerr << "Best Area: " << best_sol.second.get_area() << endl;
  cerr << "Best Wirelength: " << best_sol.second.get_wirelength() << endl;
  // Print solution to the standard output to dump it into files
  best_sol.second.print();
  _problem.print_connections();
}

// Select the best solution from the ones available in the shape function
pair<float, Floorplanning_solution> Floorplanning_solver::get_best_solution(Shape_function *slicing_tree_root) {
  Floorplanning_solution sol, best_sol;
  float best_cost = INFINITY;
  for (size_t i=0; i<slicing_tree_root->size(); ++i) {
    sol = Floorplanning_solution(slicing_tree_root, &_problem, _problem.size(), i); // A new solution is created for each point in the sf
    if (sol.cost() < best_cost) {
      best_cost = sol.cost();
      best_sol = sol;
    }
  }
  return {best_cost, best_sol};
}

