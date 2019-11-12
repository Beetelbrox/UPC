#include <iostream>
#include "floorplanning_solution.h"


using namespace std;


Floorplanning_solution::Floorplanning_solution(const std::vector<int> &npe): norm_polish_expression{npe} {

  for (int i: norm_polish_expression) { cout << i << " ";};
  cout << endl;
}
