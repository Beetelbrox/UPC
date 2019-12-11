#ifndef _FP_SOLVER_GUARD
#define _FP_SOLVER_GUARD

#include <vector>
#include <deque>
#include <memory>
#include "floorplanning_problem.h"
#include "npe.h"

class Floorplanning_solver {
  const Floorplanning_problem& _problem;
  NPE _current_npe;
  std::deque<std::pair<int, int>> _perturbations;

  void generate_random_npe(size_t n_operands, NPE& npe, bool shuffle=1);
  std::pair <int, int> calculate_floorplan_dimensions(const std::vector<int> &npe);
  std::pair<int, int> pack_npe( pair<int, int> perturbation );
  int pack_floorplans(int op, const Floorplan* fp_1, const Floorplan* fp_2, Floorplan* fp_packed);
  float calculate_cost(std::pair<int, int> perturbation={-1,-1});
  std::pair<int, float> calculate_shape_cost(std::pair<int, int> fp_dim, const std::vector<std::pair<int, int>> &positions, const std::vector<std::pair<int, int>> &shapes);
  std::pair<int, int> gen_rnd_operand_swap();
  std::pair<int, int>  gen_rnd_chain_inversion();
  std::pair<int, int> gen_rnd_operand_operator_swap();
  std::pair<int, int> gen_rnd_perturbation();
public:
  Floorplanning_solver(const Floorplanning_problem &problem);
  int solve();
};

#endif
