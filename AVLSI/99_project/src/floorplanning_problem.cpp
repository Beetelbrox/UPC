#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <utility>
#include "floorplanning_problem.h"

using namespace std;

Floorplanning_problem::Floorplanning_problem(): _module_fps(nullptr), _limit(nullptr){}

Floorplanning_problem::Floorplanning_problem(const string &filename){ from_file (filename); }

//size_t Floorplanning_problem::size() const { return module_fps.size(); }

//const Floorplan& Floorplanning_problem::get_module_floorplan (int module_id) {
//  return module_fps[module_id-1];
//}

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
  _module_fps = make_unique<Floorplan[]>(n);
  _limit = _module_fps.get() + n;

  int w, h;
  size_t module_count=0;
  for(size_t i=0; i<n;++i) {
    if ( !getline(file, line) ) break;
    vector < pair <int, int> > shape_function;
    istringstream ln_stream(line);
    while(ln_stream >> w >> h) shape_function.emplace_back(w, h);
    _module_fps[i] = Floorplan(shape_function); // This is copying structures more than it should. use allocator?
    ++module_count;
  }
  if (module_count == n) {
    cerr << "done." << endl;
    cerr << module_count << " modules read." << endl;
    return 0;
  }
  cerr << "Error: Malformed input" << endl;
  _limit = nullptr;
  _module_fps.reset(nullptr);
  return -1;
}

Floorplan* Floorplanning_problem::begin() const{ return _module_fps.get(); }

Floorplan* Floorplanning_problem::end() const{ return _limit; }

size_t Floorplanning_problem::size() const { return _limit - _module_fps.get(); }

const Floorplan* Floorplanning_problem::get_floorplan(size_t ix) const {
  if (ix >= size()) return nullptr;
  return _module_fps.get()+ix;
}
