/*########################################
 - File: floorplanning_problem.h
 - Author: Francisco Javier Jurado Moreno
 - Project: AVLSI Floorplanning project
##########################################*/

#ifndef _FP_PROB_GUARD
#define _FP_PROB_GUARD

#include <string>
#include <vector>
#include <map>
#include <utility>

class Floorplanning_problem {
  int num_modules;
  std::vector< std::vector< std::pair<int, int> > > mod_dim_list;
  std::map< std::pair<int, int>, int > distance_dict;

public:
  Floorplanning_problem(const std::string &filename);

  int get_num_modules() const;
};

#endif
