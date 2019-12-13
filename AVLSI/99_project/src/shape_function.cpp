#include <iostream>
#include <algorithm>
#include <string>
#include "shape_function.h"

using namespace std;

Shape_function::Shape_function() {}


Shape_function::Shape_function(int id, const vector<pair<int, int>> &sf_seq):
    _id(id),
    _shapes(sf_seq),
    _total_area(sf_seq[0].first*sf_seq[0].second),
    _left_child(nullptr),
    _right_child(nullptr) {}

Shape_function::Shape_function(int op, Shape_function *l_child, Shape_function *r_child);

Floorplan::dim* Floorplan::begin() const { return _shapes.get(); }

Floorplan::dim* Floorplan::end() const { return _limit; }

int Floorplan::get_id() const { return _id; };

size_t Floorplan::size() const{ return _limit - _shapes.get(); }

const Floorplan::dim Floorplan::get_shape(size_t ix) const { return _shapes[ix]; }

int Floorplan::get_shape_width(std::size_t ix) const {
  if (ix >= size()) return __INT_MAX__;
  else return _shapes[ix].first;
}

int Floorplan::get_shape_height(std::size_t ix) const {
    if (ix >= size()) return 0;
    else return _shapes[ix].second;
}

const Floorplan::dim Floorplan::get_subfp_ix(size_t ix) const { return _subfp_ix[ix]; }

const Floorplan*  Floorplan::get_left_child() const { return _left_child; }

const Floorplan* Floorplan::get_right_child() const { return _right_child; }

void Floorplan::print() const {
  for(dim* i=begin(); i !=end(); ++i) {
    cerr << _id << ": (" << i->first << "," << i->second << ") ";
  }
  cerr << endl;
  cerr << "lchild: " << ((_left_child == nullptr) ? "*" : to_string(_left_child->get_id())) << 
    " rchild: " << ((_right_child == nullptr) ? "*" : to_string(_right_child->get_id())) << endl;
}
