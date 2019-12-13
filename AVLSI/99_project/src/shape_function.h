#ifndef _SHAPE_FUNCTION_GUARD
#define _SHAPE_FUNCTION_GUARD

#include <vector>
#include <utility>

#include <memory>

class Shape_function {

  int _id, _total_area;
  std::vector<std::pair<int, int>> _shapes;
  std::vector<std::pair<std::size_t, std::size_t>> _chld_shape_ix;
  std::unique_ptr<Shape_function> _left_child, _right_child, _parent;
  void merge_children_shapes();

public:
  Shape_function(int op, std::unique_ptr<Shape_function> l_child, unique_ptr<Shape_function> r_child);
  Shape_function(int id, const std::vector<std::pair<int, int>> &sf_seq);
  const std::pair<int, int>* begin() const;
  const std::pair<int, int>* end() const;
  std::size_t size() const;
  int get_id() const;
  std::pair<int, int> get_shape(std::size_t ix) const;
  int get_shape_width(std::size_t ix) const;
  int get_shape_height(std::size_t ix) const;
  const std::pair<int, int> get_subfp_ix(std::size_t ix) const;
  Shape_function* get_left_child() const;
  Shape_function* get_right_child() const;
  void print() const;

};
#endif
