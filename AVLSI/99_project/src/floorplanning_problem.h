/*########################################
 - File: floorplanning_problem.h
 - Author: Francisco Javier Jurado Moreno
 - Project: AVLSI Floorplanning project
##########################################*/
#ifndef _FP_PROB_GUARD
#define _FP_PROB_GUARD

#include <vector>
#include <utility>
#include <unordered_map>
#include "shape_function.h"

class Floorplanning_problem {
  std::vector<std::vector<std::pair<int, int>>> _shape_function_seqs;
  std::unordered_map<size_t, std::vector<std::pair<size_t, int>>> _connections;
  size_t _n_connections;

public:
  Floorplanning_problem();
  Floorplanning_problem(const std::string &filename);

  int from_file(const std::string &filename);

  const std::vector<std::pair<int, int>>* begin() const;
  const std::vector<std::pair<int, int>>* end() const;
  std::vector<std::pair<int, int>>& operator[] (size_t ix);
  size_t size() const; // Returns the number of modules in the problem
  const std::vector<std::pair<int, int>>* get_module_sf_seq(size_t id) const;
  const std::vector<std::pair<size_t, int>>* get_module_connections(size_t id) const;
  bool module_has_connections(size_t id) const;
  void print_connections() const;
};

#endif
