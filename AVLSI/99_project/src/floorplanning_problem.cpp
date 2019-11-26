#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include "floorplanning_problem.h"

using namespace std;

Floorplanning_problem::Floorplanning_problem(const string &filename){
  ifstream file("data/" + filename);
  if(!file.is_open()) cerr << "Error: Unable to open file" << endl;
  else {
    cerr << "Reading modules from file...";
    string line;
    getline(file, line);      // Read the first line containing the number of es
    _size = stoi(line); // Parse the string into a number. Will panic if the input is malformed
    int w, h;
    for(size_t i=0; i<_size;++i) {
      if ( !getline(file, line) ) break;
            cout << "AAAb" << endl;
      vector < pair <int, int> > shape_function;
      istringstream ln_stream(line);
            cout << "AAA" << endl;
      while(ln_stream >> w >> h) shape_function.emplace_back(w, h);
            cout << "AAAx" << endl;
      _module_fps.emplace_back(shape_function);
    }
    cerr << "done" << endl;
  }
  //cout << _module_fps.size();
}

//size_t Floorplanning_problem::size() const { return module_fps.size(); }

//const Floorplan& Floorplanning_problem::get_module_floorplan (int module_id) {
//  return module_fps[module_id-1];
//}

void Floorplanning_problem::print() {

}
