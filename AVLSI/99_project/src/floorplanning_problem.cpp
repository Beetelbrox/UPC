#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include "floorplanning_problem.h"

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
  getline(file, line);      // Read the first line containing the number of es
  n = stoi(line); // Parse the string into a number
  module_fps.resize(n);

  cerr << "Reading  " << n << " modules..." << endl;
  // Read the modules and dimensions
  for(int i=0; i<n; ++i) {
    if ( !getline(file, line) ) return -1; // If an eof is read before expected assume malformed input and return, return -1
    vector < pair <int, int> > sf; // Create a vector to store the pairs
    istringstream ln_stream(line);  // Turn the line into a stream to be able to parse it easily
    while(ln_stream >> w >> h) sf.push_back({w, h});
    Floorplan fp(sf);
    module_fps[i] = fp;
  }
  return 0;
}

size_t Floorplanning_problem::size() const { return module_fps.size(); }

const Floorplan& Floorplanning_problem::get_module_floorplan (int module_id) {
  return module_fps[module_id-1];
}

void Floorplanning_problem::print() {

}
