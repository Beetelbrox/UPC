#ifndef _FP_SOLVER_GUARD
#define _FP_SOLVER_GUARD

#include <vector>
#include <deque>
#include <memory>
#include "floorplanning_problem.h"
#include "npe.h"

class Floorplanning_solver {
  const Floorplanning_problem& problem;
  std::unique_ptr<NPE> npe;
  std::deque<std::pair<int, int>> perturbations;

  void generate_random_npe(std::size_t size, std::vector<int> &npe_vec, bool shuffle=false);
  std::pair <int, int> calculate_floorplan_dimensions(const std::vector<int> &npe);
  std::pair<int, int> pack_npe();
public:
  Floorplanning_solver(const Floorplanning_problem &problem);
  int solve();
};

#endif
