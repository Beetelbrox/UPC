#include "floorplanning_problem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

using namespace std;

Floorplanning_problem::Floorplanning_problem() {}

int Floorplanning_problem::load_from_file(const std::string &filename) {
  int n, w, h;
  string line;

  // Input format: Area, min_width, max_width, min_height, max_height

  cerr << "Reading module list from input file " << filename << endl;
  ifstream file ("data/" + filename);  // This assumes a directory structure

  if(!file.is_open()) {
    cerr << "Error: Unable to open file '../data/" << filename <<
    "'. Empty problem instance generated" << endl;
    return -1;
  }
  getline(file, line);      // Read the first line containing the number of modules
  n = stoi(line); // Parse the string into a number
  module_dimensions.reserve(n);

  cerr << "Reading  " << n << " modules..." << endl;
  // Read the modules and dimensions
  for(int i=0; i<n; ++i) {
    if ( !getline(file, line) ) return -1; // If an eof is read before expected assume malformed input and return, return -1
    vector < pair <int, int> > dim; // Create a vector to store the pairs
    istringstream ln_stream(line);  // Turn the line into a stream to be able to parse it easily
    while(ln_stream >> w >> h) dim.push_back({w, h});
    module_dimensions.push_back(dim);
  }
  return 0;
}

size_t Floorplanning_problem::size() const { return module_dimensions.size(); }

vector<pair<int, int>> Floorplanning_problem::get_module_dimensions (int module_ix) const {
  return module_dimensions[module_ix];
}
