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
  std::deque<std::pair<int, int>> _perturbations;
  std::mt19937 _rng_engine;

  void build_slicing_tree();
  void calculate_block_positions ( Shape_function *sf, size_t chld_ix, std::pair<int, int> pos={0,0} );

  std::pair <int, int> calculate_floorplan_dimensions(const std::vector<int> &npe);
  //void pack_npe( std::vector<Floorplanning_solution> &solutions, std::pair<int, int> perturbation );
  //int merge_shape_functions(int op, const Floorplan* fp_1, const Floorplan* fp_2, Floorplan* fp_packed);
  //float calculate_cost(std::pair<int, int> perturbation={-1,-1}, int print_sol=0);
  std::pair<int, float> calculate_shape_cost(std::pair<int, int> fp_dim, const std::vector<std::pair<int, int>> &positions, const std::vector<std::pair<int, int>> &shapes);
  std::pair<size_t, size_t> gen_rnd_perturbation();
  std::pair<size_t, size_t> gen_rnd_operand_swap();
  std::pair<size_t, size_t>  gen_rnd_chain_inversion();
  std::pair<size_t, size_t> gen_rnd_operand_operator_swap();

public:
  Floorplanning_solver(const Floorplanning_problem &problem);
  void solve();
};

#endif
