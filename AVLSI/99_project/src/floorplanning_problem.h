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
  std::vector < std::vector< std::pair<int, int> > > module_dimensions;

public:
  Floorplanning_problem();

  int load_from_file(const std::string &filename);

  size_t size() const; // Returns the number of modules in the problem
  std::vector<std::pair<int, int>> get_module_dimensions (int module_ix) const;
};

#endif

// DO WE NEED TO HAVE DISTANCE WEIGHTS? CHECK SLIDES AND/OR ASK JORDI
