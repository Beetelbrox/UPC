#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include "floorplanning_solver.h"

using namespace std;

Floorplanning_solver::Floorplanning_solver(const Floorplanning_problem &p): problem(p) {}

Floorplanning_solver::~Floorplanning_solver() {
  for(Floorplanning_solution& sol : solutions ) { delete &sol; }
}

int Floorplanning_solver::solve() {
  generate_initial_solution();
  return 0;
}


void Floorplanning_solver::generate_initial_solution() {

  vector<int> node_shuffle, initial_pe(2*problem.get_num_modules()-1, 0);
  int n_operands=1, n_operators=0;
  for (int i=1; i<=problem.get_num_modules(); ++i) node_shuffle.push_back(i);
  random_shuffle(node_shuffle.begin(), node_shuffle.end());
  initial_pe[0] = node_shuffle[0];  // Always insert the first element (assumes the problem is not empty)
  while(n_operators < problem.get_num_modules()-1) {
    cout << n_operands << " "<<  n_operators << endl;
    // By trying to always push operands first we maintain the balloting property
    if (n_operands < 2*(n_operators+1) && n_operands < problem.get_num_modules()) { // Try to make the tree as even as possible
      initial_pe[n_operands+n_operators] = node_shuffle[n_operands];
      ++n_operands;
    } else {
      initial_pe[n_operands+n_operators] = initial_pe[n_operands+n_operators-1] < 0 ? ((initial_pe[n_operands+n_operators-1]+3)%2)-2 : V;
      ++n_operators;
    }
  }

  Floorplanning_solution * sol = new Floorplanning_solution(initial_pe);
  solutions.push_back(*sol);
}

int Floorplanning_solver::generate_perturbation(const Floorplanning_solution &current) {
  
}
