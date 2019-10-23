#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "graph.h"


int main() {
  int N, M = 0;
  // Arabic 21532 68767
  // Basque 12207 25558
  M = 25558;
  N = 12207;
  std::cout << "## Building Basque ##" << std::endl;
  Graph g1 = Graph(M, N);
  std::cout << "Number of vertex: " << g1.get_num_vertices() << std::endl;
  std::cout << "Number of edges: " << g1.get_num_edges() << std::endl;
  g1.closeness_centrality(2);
  //std::cout << "Geodesic distance 1 -> 10000: " << g1.geodesic_distance(1,10000) << std::endl;
}
