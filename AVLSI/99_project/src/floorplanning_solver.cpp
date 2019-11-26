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
