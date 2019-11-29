#ifndef _FP_GUARD
#define _FP_GUARD

#include <vector>
#include <utility>
#include <memory>

class Floorplan {
  typedef std::pair<int, int> dim;

  std::unique_ptr<dim[]> _shapes;
  dim *_limit;
  int _x, _y;
  const Floorplan *_left_child, *_right_child;

public:
  Floorplan(const std::vector<dim> &sf=std::vector<dim>(), int x=0, int y=0, const Floorplan* left_child=nullptr, const Floorplan* right_child=nullptr);
  Floorplan(const dim* sf, int num_shapes, int x=0, int y=0, const Floorplan* left_child=nullptr, const Floorplan* right_child=nullptr);

  dim* begin() const;
  dim* end() const;
  std::size_t size() const;
  dim get_position() const;
  int get_x() const;
  int get_y() const;
  const dim* get_shape(std::size_t ix) const;
  const Floorplan* get_left_child() const;
  const Floorplan* get_right_child() const;
  void print() const;

};
#endif
