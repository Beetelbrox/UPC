#ifndef _FP_SOLVER_GUARD
#define _FP_SOLVER_GUARD

#include "floorplanning_problem.h"
#include "floorplanning_solution.h"

#include <vector>

class Floorplanning_solver {
  const Floorplanning_problem& problem;
  std::vector<Floorplanning_solution> solutions;

  void generate_initial_solution();

public:
  static const int V = -1, H = -2;
  Floorplanning_solver(const Floorplanning_problem &problem);
  ~Floorplanning_solver();
  int solve();
};

#endif
