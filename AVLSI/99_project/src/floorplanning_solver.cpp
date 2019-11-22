#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <stack>
#include "floorplanning_solver.h"

using namespace std;

Floorplanning_solver::Floorplanning_solver(const Floorplanning_problem &p): problem(p) {}

Floorplanning_solver::~Floorplanning_solver() {
  for( Floorplanning_solution& sol : solutions ) { delete &sol; }
}

int Floorplanning_solver::solve() {
  generate_initial_solution(problem.size());
  calculate_floorplan_dimensions(solutions.back().get_npe());
  return 0;
}

// Refactor this to make it random without the weird constraints
void Floorplanning_solver::generate_initial_solution(int size) {
  cerr << "Generating initial solution...";

  vector<int> node_shuffle, initial_npe;
  size_t n_operators=0, n_operands=0, next_insert;
  // Create a sequence of indices and shuffle it
  for (size_t i=1; i<=problem.size(); ++i) node_shuffle.push_back(i);
  random_shuffle(node_shuffle.begin(), node_shuffle.end());

  while(n_operators+1 < problem.size()) {
    if (n_operands >= problem.size()) next_insert = 1;       // All operands inserted
    else if (n_operators+1 >= n_operands) next_insert = 0;  // Balloting rule
    else next_insert = rand()%2;

    // 1 insert operator, 0 insert operand
    if(next_insert) {
      initial_npe.push_back(initial_npe[n_operands+n_operators-1] < 0 ? !(initial_npe[n_operands+n_operators-1] + 2) - 2 : (rand()%2)-2);
      ++n_operators;
    } else {
      initial_npe.push_back(node_shuffle[n_operands++]);
    }
  }

  Floorplanning_solution * sol = new Floorplanning_solution(initial_npe);
  solutions.push_back(*sol);
  cerr << "done." << endl;
  sol->print();
}

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
      for (pair <int, int> p : submod_1) {

      }
    } else {
      cerr << "Error: Bad index in NPE." << endl;
      return {-1,-1};
    }

  }
  return {0,0};
}
