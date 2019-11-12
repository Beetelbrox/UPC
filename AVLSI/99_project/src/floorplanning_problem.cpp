#include "floorplanning_problem.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

Floorplanning_problem::Floorplanning_problem(const std::string &filename) {
  cerr << "Trying to read file " <<  filename << "..." << endl;
  ifstream file ("data/" + filename);  // This assumes a directory structure
  string line;
  int w, h, m1, m2, d;
  if(file.is_open()) {
    getline(file, line);      // Read the first line containing the number of modules
    num_modules = stoi(line); // Parse the string into a number
    cerr << "Reading  " << num_modules << " modules..." << endl;
    // Read the modules and dimensions
    for(int i=0; i<num_modules; ++i) {
      if ( !getline(file, line) ) break; // If an eof is read, jump out of the loop
      vector< pair<int, int> > v;
      istringstream ln_stream(line);  // Turn the line into a stream to be able to parse it easily
      while(ln_stream >> w >> h) v.push_back(pair<int, int>(w,h));
      mod_dim_list.push_back(v);
    }
    // Read the distances
    while(getline(file, line)) {
      istringstream ln_stream(line);
      ln_stream >> m1 >> m2 >> d;
      distance_dict[{m1, m2}] = d;
    }
  } else {
    cerr << "Error: Unable to open file '../data/" << filename <<
    "'. Empty problem instance generated" << endl;
  }
}

int Floorplanning_problem::get_num_modules() const { return num_modules; }
