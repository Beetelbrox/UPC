#include <iostream>
#include <algorithm>
#include "shape_function.h"
#include "npe.h"

using namespace std;
Shape_function::Shape_function():
  _id(0),
  _left_child(nullptr),
  _right_child(nullptr)
{}

Shape_function::Shape_function(int id, const vector<pair<int, int>> &sf_seq):
    _id(id),
    _min_area(sf_seq[0].first*sf_seq[0].second),
    _shapes(sf_seq),
    _left_child(nullptr),
    _right_child(nullptr)
    {}

Shape_function::Shape_function(int op, Shape_function* l_child, Shape_function* r_child):
  _id(op),
  _min_area(l_child->_min_area + r_child->_min_area),
  _left_child(l_child),  // Use std::move to convert to an rvalue and transfer ownership
  _right_child(r_child)
{
  merge_children_shapes();
}

void Shape_function::merge_children_shapes() {

  size_t ix_1, ix_2;
  bool done;
  pair<int, int> s_1, s_2;
  if ( _id == NPE::H ) {
    ix_1 = _left_child->size()-1;
    ix_2 = _right_child->size()-1;
    while( !done ){
      s_1 = _left_child->get_shape(ix_1);
      s_2 = _right_child->get_shape(ix_2);
      _shapes.emplace_back(max(s_1.first, s_2.first), s_1.second + s_2.second);
      _chld_shape_ix.emplace_back(ix_1, ix_2);
      if (ix_1 == 0 ||  ix_2 == 0) done = true; // If we are on the last element of any of the lists
      if (s_1.first > s_2.first) --ix_1;
      else if (s_1.first < s_2.first) -- ix_2;
      else {
        --ix_1;
        --ix_2;
      }
    }
    // Reverse the lists of points and coordinates to keep them sorted in increasing width
    reverse(_shapes.begin(), _shapes.end());
    reverse(_chld_shape_ix.begin(), _chld_shape_ix.end());
  } else if ( _id == NPE::V ) {
    ix_1 = 0;
    ix_2 = 0;
    while( !done ) {
      s_1 = _left_child->get_shape(ix_1);
      s_2 = _right_child->get_shape(ix_2);
      _shapes.emplace_back(s_1.first + s_2.first, max(s_1.second, s_2.second));
      _chld_shape_ix.emplace_back(ix_1, ix_2);
      if ( s_1.second > s_2.second ) ++ix_1;
      else if ( s_1.second < s_2.second ) ++ix_2;
      else {
        ++ix_1;
        ++ix_2;
      }
      if (ix_1 >= _left_child->size() ||  ix_2 >= _right_child->size() ) done = true; // If we are on the last element of any of the lists
    }
  } else cerr << "Shape function Merging: Malformed NPE" << endl; // Panic
}

const pair<int, int>* Shape_function::begin() const { return &*_shapes.begin(); }
const pair<int, int>* Shape_function::end() const { return &*_shapes.end(); }
int Shape_function::get_id() const { return _id; };
size_t Shape_function::size() const { return _shapes.size(); }
int Shape_function::get_min_area() const { return _min_area; }
int Shape_function::get_area(size_t ix) const { return _shapes[ix].first*_shapes[ix].second; }

pair<int, int> Shape_function::get_shape(size_t ix) const { return _shapes[ix]; }

pair<size_t, size_t> Shape_function::get_child_sf_ix(std::size_t ix) const { return _chld_shape_ix[ix]; }
Shape_function*  Shape_function::get_left_child() const { return _left_child; }
Shape_function* Shape_function::get_right_child() const { return _right_child; }

void Shape_function::print() const {
  cerr << _id << ": ";
  for(const pair<int, int>* i=begin(); i !=end(); ++i) {
    cerr << "(" << i->first << "," << i->second << ") ";
  }
  cerr << endl;
  cerr << "lchild: " << ((_left_child == nullptr) ? "*" : to_string(_left_child->get_id())) << 
    " rchild: " << ((_right_child == nullptr) ? "*" : to_string(_right_child->get_id())) << endl;
}
