#include <iostream>
#include "floorplan.h"

using namespace std;

Floorplan::Floorplan(): pos_x{0}, pos_y{0}, shape_function() {}

Floorplan::Floorplan(vector<pair<int, int>> sf, int x, int y):
  pos_x{x}, pos_y{y}, shape_function(sf)  {}

const std::vector<std::pair<int, int>>* Floorplan::get_shape_function () const {
  return &shape_function;
}
