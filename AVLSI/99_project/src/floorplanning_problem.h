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
#include "floorplan.h"

class Floorplanning_problem {
  std::vector<Floorplan> _module_fps;
  size_t _size;

public:
  Floorplanning_problem(const std::string &filename);

  size_t size() const; // Returns the number of modules in the problem
  //Floorplan& get_module_floorplan (int module_id) const;

  void print();
};

#endif

// DO WE NEED TO HAVE DISTANCE WEIGHTS? CHECK SLIDES AND/OR ASK JORDI
