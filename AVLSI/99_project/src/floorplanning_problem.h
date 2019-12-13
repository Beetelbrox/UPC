/*########################################
 - File: floorplanning_problem.h
 - Author: Francisco Javier Jurado Moreno
 - Project: AVLSI Floorplanning project
##########################################*/

#ifndef _FP_PROB_GUARD
#define _FP_PROB_GUARD

#include <vector>
#include <utility>
#include "shape_function.h"

class Floorplanning_problem {
  std::vector<std::vector<std::pair<int, int>>> _shape_function_seqs;

public:
  Floorplanning_problem();
  Floorplanning_problem(const std::string &filename);

  int from_file(const std::string &filename);

  const std::vector<std::pair<int, int>>* begin() const;
  const std::vector<std::pair<int, int>>* end() const;
  std::vector<std::pair<int, int>>& operator[] (size_t ix);
  size_t size() const; // Returns the number of modules in the problem
  const std::vector<std::pair<int, int>>* get_module_sf_seq(size_t id) const;
};

#endif
