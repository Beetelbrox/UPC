#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <utility>
#include "floorplanning_problem.h"

using namespace std;

Floorplanning_problem::Floorplanning_problem(): _module_fps() {}

Floorplanning_problem::Floorplanning_problem(const string &filename): _module_fps() { from_file (filename); }

int Floorplanning_problem::from_file(const string &filename) {
  ifstream file("data/" + filename);
  if(!file.is_open()) {
    cerr << "Error: Unable to open file '" << filename << "'" << endl;
    return -1;
  }
  cerr << "Reading modules from file '" << filename << "'" << "...";
  string line;
  getline(file, line);      // Read the first line containing the number of modules
  size_t n = stoi(line);           // Parse the string into a number. Will panic if the input is malformed

  int w, h;
  for(size_t i=0; i<n;++i) {
    if ( !getline(file, line) ) break;
    vector < pair <int, int> > shape_function;
    istringstream ln_stream(line);
    while(ln_stream >> w >> h) shape_function.emplace_back(w, h);
    sort(shape_function.begin(), shape_function.end()); // Sort the shapes by the x coordinate. This will be useful when packing floorplans
    _module_fps.emplace_back(i+1, shape_function);
  }
  if (_module_fps.size() == n) {
    cerr << "done." << endl;
    cerr << _module_fps.size() << " modules read." << endl;
    return 0;
  }
  cerr << "Error: Malformed input" << endl;
  _module_fps.clear();
  return -1;
}

const Floorplan* Floorplanning_problem::begin() const{ return &*_module_fps.begin(); }

const Floorplan* Floorplanning_problem::end() const{ return &*_module_fps.end(); }

size_t Floorplanning_problem::size() const { return _module_fps.size(); }

const Floorplan* Floorplanning_problem::get_floorplan(size_t id) const {
  if (id > size() || id <= 0) return nullptr;
  return &_module_fps[id-1];
}
