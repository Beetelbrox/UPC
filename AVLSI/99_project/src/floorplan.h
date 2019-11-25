#ifndef _FP_GUARD
#define _FP_GUARD

#include <vector>
#include <utility>

class Floorplan {
  int pos_x, pos_y, w, h;
  std::vector<std::pair<int, int>> shape_function;

public:
  Floorplan();
  Floorplan(std::vector<std::pair<int, int>> sf, int x=0, int y=0);

  const std::vector<std::pair<int, int>>* get_shape_function () const; 

  //std::pair<int, int> get_dim() const { return {x, y}; }

};
#endif
