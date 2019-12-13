#ifndef _SHAPE_FUNCTION_GUARD
#define _SHAPE_FUNCTION_GUARD

#include <vector>
#include <utility>

class Shape_function {

  int _id, _total_area;
  std::vector<std::pair<int, int>> _shapes;
  Shape_function *_left_child, *_right_child;

public:
  Shape_function();
  Shape_function(int op, const Shape_function *l_child, Shape_function *r_child);
  Shape_function(int id, const std::vector<std::pair<int, int>> &sf_seq);
  std::pair<int, int>* begin() const;
  std::pair<int, int>* end() const;
  std::size_t size() const;
  int get_id() const;
  const std::pair<int, int> get_shape(std::size_t ix) const;
  int get_shape_width(std::size_t ix) const;
  int get_shape_height(std::size_t ix) const;
  const std::pair<int, int> get_subfp_ix(std::size_t ix) const;
  const Shape_function* get_left_child() const;
  const Shape_function* get_right_child() const;
  void print() const;

};
#endif
