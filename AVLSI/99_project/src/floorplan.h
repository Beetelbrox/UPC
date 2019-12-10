#ifndef _FP_GUARD
#define _FP_GUARD

#include <vector>
#include <utility>
#include <memory>

class Floorplan {
  typedef std::pair<int, int> dim;

  int _id;
  std::unique_ptr<dim[]> _shapes, _subfp_ix;
  dim *_limit;
  const Floorplan *_left_child, *_right_child;

public:
  Floorplan();
  Floorplan(int id, const std::vector<dim> &sf, std::vector <dim> subfp_ix=std::vector<dim>(), const Floorplan* left_child=nullptr, const Floorplan* right_child=nullptr);
  Floorplan(int id, const dim* shapes, int n_shapes, dim* subfp_ix=nullptr, const Floorplan* left_child=nullptr, const Floorplan* right_child=nullptr);

  dim* begin() const;
  dim* end() const;
  std::size_t size() const;
  int get_id() const;
  const dim get_shape(std::size_t ix) const;
  int get_shape_width(std::size_t ix) const;
  int get_shape_height(std::size_t ix) const;
  const dim get_subfp_ix(std::size_t ix) const;
  const Floorplan* get_left_child() const;
  const Floorplan* get_right_child() const;
  void print() const;

};
#endif
