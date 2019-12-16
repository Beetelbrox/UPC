/*########################################
 - File: floorplanning_solution.h
 - Author: Francisco Javier Jurado Moreno
 - Project: AVLSI Floorplanning project
##########################################*/
#ifndef _SHAPE_FUNCTION_GUARD
#define _SHAPE_FUNCTION_GUARD

#include <vector>
#include <utility>

class Shape_function {

  int _id, _min_area;
  std::vector<std::pair<int, int>> _shapes;
  std::vector<std::pair<std::size_t, std::size_t>> _chld_shape_ix;
  Shape_function *_left_child, *_right_child, *_parent;
  void merge_children_shapes();

public:
  Shape_function();
  explicit Shape_function(int op, Shape_function* l_child, Shape_function* r_child);
  explicit Shape_function(int id, const std::vector<std::pair<int, int>> &sf_seq);
  const std::pair<int, int>* begin() const;
  const std::pair<int, int>* end() const;
  std::size_t size() const;
  int get_id() const;
  int get_min_area() const;
  int get_area(size_t ix) const;
  std::pair<int, int> get_shape(size_t ix) const;
  std::pair<size_t, size_t> get_child_sf_ix(size_t ix) const;
  Shape_function* get_left_child();
  Shape_function* get_right_child();
  Shape_function* get_parent();
  void set_parent(Shape_function* parent);
  void print() const;

};
#endif
