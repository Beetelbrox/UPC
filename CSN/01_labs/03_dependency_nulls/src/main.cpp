#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "graph.h"


int main() {
  int N, M = 0;
  // Arabic 21532 68767
  M = 68767;
  N = 21532;
  Graph g1 = Graph(M, N);
  std::cout << "Number of vertex: " << g1.get_num_vertices() << std::endl;
  std::cout << "Number of edges: " << g1.get_num_edges() << std::endl;
}
