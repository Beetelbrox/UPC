#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <utility>
#include "floorplanning_problem.h"

using namespace std;

Floorplanning_problem::Floorplanning_problem() {}

Floorplanning_problem::Floorplanning_problem(const string &filename) { from_file (filename); }

int Floorplanning_problem::from_file(const string &filename) {
  ifstream file("data/" + filename);
  if(!file.is_open()) {
    cerr << "Error: Unable to open file '" << filename << "'" << endl;
    exit(EXIT_FAILURE);
  }
  cerr << "Reading modules from file '" << filename << "'" << "...";
  string line;
  getline(file, line);              // Read the first line containing the number of modules
  size_t n = stoi(line);            // Parse the string into a number. Will panic if the input is malformed
  _shape_function_seqs.resize(n);   // Resize the sf seq vector to the appropriate size

  int w, h;
  for(size_t i=0; i<n;++i) {
    if ( !getline(file, line) ) break;
    istringstream ln_stream(line);
    
    while(ln_stream >> w >> h) _shape_function_seqs[i].emplace_back(w, h);
    sort(_shape_function_seqs[i].begin(), _shape_function_seqs[i].end()); // Sort the shapes by the x coordinate. This will be useful when packing floorplans
  }
  if (_shape_function_seqs.size() == n) {
    cerr << "done." << endl;
    cerr << _shape_function_seqs.size() << " modules read." << endl;
    return 0;
  }
  cerr << "Problem reading Error: Malformed input" << endl;
  exit(0);
}

vector<pair<int, int>>& Floorplanning_problem::operator[] (int ix) {
  if (ix >= size()) {
    cerr << "Error [Floorplanning Problem]: Array index out of bounds" << endl;
    exit(EXIT_FAILURE);
  }
  return _shape_function_seqs[ix];
}

const vector<pair<int, int>>* Floorplanning_problem::begin() const{ return &*_shape_function_seqs.begin(); }

const vector<pair<int, int>>* Floorplanning_problem::end() const{ return &*_shape_function_seqs.end(); }

size_t Floorplanning_problem::size() const { return _shape_function_seqs.size(); }

const vector<pair<int, int>>* Floorplanning_problem::get_module_sf_seq(size_t id) const {
  if (id > size()) {
    cerr << "Error [Floorplanning Problem]: Module id out of bounds" << endl;
    exit(EXIT_FAILURE);
  }
  return &_shape_function_seqs[id-1];
}
