#ifndef _FP_SOLUTION_GUARD
#define _FP_SOLUTION_GUARD

#include <vector>

class Floorplanning_solution {
  std::vector<int> norm_polish_expression;   // We can store the polish expression as a regular vetor of ints to save space
public:
  Floorplanning_solution(const std::vector<int> &npe);
};
#endif
