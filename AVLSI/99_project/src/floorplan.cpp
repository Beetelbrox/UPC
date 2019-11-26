#include <iostream>
#include <algorithm>
#include "floorplan.h"

using namespace std;

Floorplan::Floorplan(const vector<dim> &sf, int x, int y):
  Floorplan(&sf[0], sf.size(), x, y) {}

Floorplan::Floorplan(const dim* sf, int n_shapes, int x, int y): dim_x{x}, _y{y} {
    cout << sf << endl;
    _shape_function = new dim[n_shapes];
    cout <<"teste" << endl;
    _limit = _shape_function + n_shapes;

    copy(sf, sf+n_shapes, _shape_function);
  }

Floorplan::~Floorplan() { delete[] _shape_function; }

// const std::vector<std::pair<int, int>>* Floorplan::get_shape_function () const {
//   return &shape_function;
// }

Floorplan::dim* Floorplan::begin() const{
  return _shape_function;
}
Floorplan::dim* Floorplan::end() const{
  return _limit;
}

size_t Floorplan::get_num_shapes() {
  return _limit - _shape_function;
}

void Floorplan::print() {
  cerr << "x = " << _x << ", y = " << _y << endl;
  for(dim* i=begin(); i !=end(); ++i) {
    cerr << "(" << i->first << "," << i->second << ") ";
  }
  cerr << endl;
}
