/*########################################
 - File: floorplanning_problem.h
 - Author: Francisco Javier Jurado Moreno
 - Project: AVLSI Floorplanning project
##########################################*/

#ifndef _FP_PROB_GUARD
#define _FP_PROB_GUARD

#include <string>
#include <memory>
#include "floorplan.h"

class Floorplanning_problem {
  std::vector<Floorplan> _module_fps;

public:
  Floorplanning_problem();
  Floorplanning_problem(const std::string &filename);

  int from_file(const std::string &filename);

  const Floorplan* begin() const;
  const Floorplan* end() const;
  size_t size() const; // Returns the number of modules in the problem
  const Floorplan* get_floorplan(size_t id) const;
};

#endif

// DO WE NEED TO HAVE DISTANCE WEIGHTS? CHECK SLIDES AND/OR ASK JORDI
