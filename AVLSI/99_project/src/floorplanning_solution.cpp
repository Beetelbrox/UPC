#include <iostream>
#include "floorplanning_solution.h"


using namespace std;


Floorplanning_solution::Floorplanning_solution(const std::vector<int> &npe): norm_polish_expression{npe} {

  for (int i: norm_polish_expression) { cout << i << " ";};
  cout << endl;
}

int Floorplanning_solution::get_width(){ return width; }
int Floorplanning_solution::get_height(){ return height; }
