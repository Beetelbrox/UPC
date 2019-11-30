#include <iostream>
#include <algorithm>
#include "floorplan.h"

using namespace std;

Floorplan::Floorplan() {}


Floorplan::Floorplan(int id, const vector<dim> &sf, vector <dim> subfp_ix, const Floorplan* left_child, const Floorplan* right_child):
  Floorplan(id, &sf[0], sf.size(), &subfp_ix[0], left_child, right_child) {}

Floorplan::Floorplan(int id, const dim* shapes, int n_shapes, dim* subfp_ix, const Floorplan* left_child, const Floorplan* right_child):
   _id(id), _subfp_ix(nullptr), _left_child(left_child), _right_child(right_child){
    _shapes = make_unique<dim[]>(n_shapes);
    _limit = _shapes.get() + n_shapes;  // pointer to the end
    copy(shapes, shapes+n_shapes, _shapes.get());
    if (subfp_ix != nullptr) {
      _subfp_ix = make_unique<dim[]>(n_shapes);
      copy(subfp_ix, subfp_ix+n_shapes, _subfp_ix.get());
    }
  }

Floorplan::dim* Floorplan::begin() const { return _shapes.get(); }

Floorplan::dim* Floorplan::end() const { return _limit; }

int Floorplan::get_id() const { return _id; };

size_t Floorplan::size() const{ return _limit - _shapes.get(); }

const Floorplan::dim Floorplan::get_shape(size_t ix) const { return _shapes[ix]; }

const Floorplan::dim Floorplan::get_subfp_ix(size_t ix) const { return _subfp_ix[ix]; }

const Floorplan*  Floorplan::get_left_child() const { return _left_child; }

const Floorplan* Floorplan::get_right_child() const { return _right_child; }

void Floorplan::print() const {
  for(dim* i=begin(); i !=end(); ++i) {
    cerr << "(" << i->first << "," << i->second << ") ";
  }
  cerr << endl;
}
