#include <iostream>
#include <algorithm>
#include "shape_function.h"
#include "npe.h"

using namespace std;

Shape_function::Shape_function(int id, const vector<pair<int, int>> &sf_seq):
    _id(id),
    _shapes(sf_seq),
    _total_area(sf_seq[0].first*sf_seq[0].second),
    _left_child(nullptr),
    _right_child(nullptr),
    _parent(nullptr) {}

Shape_function::Shape_function(int op, std::unique_ptr<Shape_function> l_child, unique_ptr<Shape_function> r_child):
  _id(op),
  _total_area(_left_child->_total_area + _right_child->_total_area),
  _left_child(std::move(l_child)),  // Use std::move to convert to an rvalue and transfer ownership
  _right_child(std::move(r_child)),
  _parent(nullptr)
{
  _left_child->_parent.reset(this);
  _right_child->_parent.reset(this);
  merge_children_shapes();
}

void Shape_function::merge_children_shapes() {
  size_t ix_1, ix_2;
  pair<int, int> s_1, s_2;
  if ( _id == NPE::H ) {
    ix_1 = _left_child->size();
    ix_2 = _right_child->size();
    while( ix_1 > 0 && ix_2 > 0 )
      s_1 = _left_child->get_shape(ix_1);
      s_2 = _right_child->get_shape(ix_2);
      _shapes.emplace_back(max(s_1.first, s_2.first), s_1.second + s_2.second);
      _chld_shape_ix.emplace_back(ix_1, ix_2);
      if (s_1.first > s_2.first) --ix_1;
      else if (s_1.first < s_2.first) -- ix_2;
      else {
        --ix_1;
        --ix_2;
      }
    // Reverse the lists of points and coordinates to keep them sorted in increasing width
    reverse(_shapes.begin(), _shapes.end());
    reverse(_chld_shape_ix.begin(), _chld_shape_ix.end());
  } else if ( _id == NPE::V ) {
    ix_1 = 0;
    ix_2 = 0;
    while(ix_1 < _left_child->size() && ix_2 < _right_child->size()) {
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
    }
  } else cerr << "Shape function Merging: Malformed NPE" << endl; // Panic
}

const pair<int, int>* Shape_function::begin() const { return &*_shapes.begin(); }

const pair<int, int>* Shape_function::end() const { return &*_shapes.end(); }

int Shape_function::get_id() const { return _id; };

size_t Shape_function::size() const{ return _shapes.size(); }

pair<int, int> Shape_function::get_shape(size_t ix) const { return _shapes[ix]; }

int Shape_function::get_shape_width(std::size_t ix) const {
  if (ix >= size()) return __INT_MAX__;
  else return _shapes[ix].first;
}

int Shape_function::get_shape_height(std::size_t ix) const {
    if (ix >= size()) return 0;
    else return _shapes[ix].second;
}

const pair<int, int> Shape_function::get_subfp_ix(size_t ix) const { return _chld_shape_ix[ix]; }

Shape_function*  Shape_function::get_left_child() const { return _left_child.get(); }

Shape_function* Shape_function::get_right_child() const { return _right_child.get(); }

void Shape_function::print() const {
  for(const pair<int, int>* i=begin(); i !=end(); ++i) {
    cerr << _id << ": (" << i->first << "," << i->second << ") ";
  }
  cerr << endl;
  cerr << "lchild: " << ((_left_child == nullptr) ? "*" : to_string(_left_child->get_id())) << 
    " rchild: " << ((_right_child == nullptr) ? "*" : to_string(_right_child->get_id())) << endl;
}
