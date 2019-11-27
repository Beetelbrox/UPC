#include <iostream>
#include <algorithm>
#include "floorplan.h"

using namespace std;

Floorplan::Floorplan(const vector<dim> &sf, int x, int y):
  Floorplan(&sf[0], sf.size(), x, y) {}

Floorplan::Floorplan(const dim* shapes, int n_shapes, int x, int y): _x(x), _y(y) {
    _shapes = make_unique<dim[]>(n_shapes);
    _limit = _shapes.get() + n_shapes;  // pointer to the end
    copy(shapes, shapes+n_shapes, _shapes.get());
  }

Floorplan::dim* Floorplan::begin() const { return _shapes.get(); }

Floorplan::dim* Floorplan::end() const { return _limit; }

size_t Floorplan::size() const{ return _limit - _shapes.get(); }

Floorplan::dim Floorplan::get_position() const { return {_x, _y}; }

int Floorplan::get_x() const { return _x; }

int Floorplan::get_y() const { return _y; }

const Floorplan::dim* Floorplan::get_shape(size_t ix) const {
  if (ix >= size()) return nullptr;
  return _shapes.get()+ix;
}

void Floorplan::print() const {
  cerr << "x = " << _x << ", y = " << _y << endl;
  for(dim* i=begin(); i !=end(); ++i) {
    cerr << "(" << i->first << "," << i->second << ") ";
  }
  cerr << endl;
}