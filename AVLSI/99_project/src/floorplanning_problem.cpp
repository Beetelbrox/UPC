/*########################################
 - File: floorplanning_problem.cpp
 - Author: Francisco Javier Jurado Moreno
 - Project: AVLSI Floorplanning project
##########################################*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <utility>
#include "floorplanning_problem.h"

using std::cerr;
using std::endl;
using std::pair;
using std::string;
using std::vector;

Floorplanning_problem::Floorplanning_problem(): _n_connections(0) {}

Floorplanning_problem::Floorplanning_problem(const string &filename): _n_connections(0) { from_file (filename); }

int Floorplanning_problem::from_file(const string &filename) {

  std::ifstream file("data/" + filename);
  if(!file.is_open()) {
    cerr << "Error: Unable to open file '" << filename << "'" << endl;
    exit(EXIT_FAILURE);
  }
    cerr << "AA" << endl;
  cerr << "Reading modules from file '" << filename << "'" << "...";
  string line;
  getline(file, line);              // Read the first line containing the number of modules
  size_t n = stoi(line);            // Parse the string into a number. Will panic if the input is malformed
  _shape_function_seqs.resize(n);   // Resize the sf seq vector to the appropriate size

  int w, h;
  for(size_t i=0; i<n;++i) {
    if ( !getline(file, line) ) break;
    std::istringstream ln_stream(line);
    
    while(ln_stream >> w >> h) _shape_function_seqs[i].emplace_back(w, h);
    sort(_shape_function_seqs[i].begin(), _shape_function_seqs[i].end()); // Sort the shapes by the x coordinate. This will be useful when packing floorplans
  }
  cerr << "done." << endl;
  cerr << _shape_function_seqs.size() << " modules read." << endl;
  if (_shape_function_seqs.size() < n) {
    cerr << "Error [Problem]: Malformed input file - Wrong number of modules" << endl;
    exit(EXIT_FAILURE);
  }
  cerr << "Reading connections from file '" << filename << "'" << "...";
  getline(file, line);       // Read the number of connections
  n = stoi(line);            // Parse the string into a number. Will panic if the input is malformed
  int b1, b2, wires;
  for(size_t i=0; i<n;++i) {
    if ( !getline(file, line) ) break;
    std::istringstream ln_stream(line);
    ln_stream >> b1 >> b2 >> wires;
    if (_connections.find(b1) != _connections.end()) {
      _connections.emplace(b1, 0);
    }
    _connections[b1].push_back({b2, wires});
    ++_n_connections;
  }
  cerr << "done." << endl;
  cerr << _n_connections << " connections read." << endl;
  if (_n_connections < n) {
    cerr << "Error [Problem]: Malformed input file - Wrong number of connections" << endl;
    exit(EXIT_FAILURE);
  }
  return 0;
}

vector<pair<int, int>>& Floorplanning_problem::operator[] (size_t ix) {
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

bool Floorplanning_problem::module_has_connections(size_t id) const {
  return _connections.find(id) != _connections.end();
}

const vector<pair<size_t, int>>* Floorplanning_problem::get_module_connections(size_t id) const {
  if (_connections.find(id) != _connections.end()) {
    return &_connections.find(id)->second;
  } else {
    return nullptr;
  }
  
}

void Floorplanning_problem::print_connections() const {
  std::cout << _n_connections << endl;
  for (size_t i=1; i <= size(); ++i) {
    if (module_has_connections(i)) {
      for(pair<size_t, int> p : _connections.find(i)->second) {
        std::cout << i << " " << p.first << " " << p.second << endl;
      }
    }
  }
}
