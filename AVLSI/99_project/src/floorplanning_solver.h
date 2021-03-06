/*########################################
 - File: floorplanning_solver.h
 - Author: Francisco Javier Jurado Moreno
 - Project: AVLSI Floorplanning project
##########################################*/
#ifndef _FP_SOLVER_GUARD
#define _FP_SOLVER_GUARD

#include <vector>
#include <deque>
#include <memory>
#include <random>
#include "npe.h"
#include "floorplanning_problem.h"
#include "shape_function.h"
#include "floorplanning_solution.h"

class Floorplanning_solver {
  const Floorplanning_problem& _problem;
  NPE _npe;
  std::vector<Shape_function> _slicing_tree;
  Shape_function *_slicing_tree_root;

  Shape_function* build_slicing_tree(NPE &npe, std::vector<Shape_function> &sl_tree);
  void calculate_block_positions ( Shape_function *sf, size_t chld_ix, std::pair<int, int> pos={0,0} );
  std::pair<float, Floorplanning_solution> get_best_solution(Shape_function *slicing_tree_root);

public:
  Floorplanning_solver(const Floorplanning_problem &problem);
  void solve();
};

#endif
