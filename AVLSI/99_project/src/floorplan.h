#ifndef _FP_GUARD
#define _FP_GUARD

#include <vector>
#include <utility>

class Floorplan {
  typedef std::pair<int, int> dim;
  dim *_shape_function, *_limit;
  int _x, _y;

public:

  explicit Floorplan(const std::vector<dim> &sf=std::vector<dim>(), int x=0, int y=0);
  Floorplan(const dim* sf, int num_shapes, int x=0, int y=0);
  ~Floorplan();

  dim* begin() const;
  dim* end() const;

  std::size_t get_num_shapes();

  void print();

  //const std::vector<std::pair<int, int>>* get_shape_function () const;

  //std::pair<int, int> get_dim() const { return {x, y}; }

};
#endif
