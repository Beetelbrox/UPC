/*########################################
 - File: floorplanning_problem.h
 - Author: Francisco Javier Jurado Moreno
 - Project: AVLSI Floorplanning project
##########################################*/

#ifndef _FP_PROB_GUARD
#define _FP_PROB_GUARD

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

class Floorplanning_problem {
  int num_modules;
  std::vector< std::vector< std::pair<int, int> > > mod_dim_list;
  std::unordered_map< std::pair<int, int>, int > distance_dict;

public:
  static const int V = -1, H = -2;
  Floorplanning_problem(const std::string &filename);
};

#endif
