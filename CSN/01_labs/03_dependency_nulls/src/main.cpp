#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "graph.h"


int main() {
  const std::string DATA_DIR = "data/";
  const std::string path = DATA_DIR + "Basque_syntactic_dependency_network.txt";
  bool test_switch = true;
  if (!test_switch){
    int N[10] = {21532, 12207, 36865, 40298, 69303, 29634, 13283, 36126, 14726, 20409};  
    int M[10] = {68767, 25558, 197318, 181081, 257295, 193186, 43974, 106716, 56042, 45642};
    std::string L[10] = {"Arabic", "Basque", "Catalan", "Chinese", "Czech", "English", "Greek", "Hungarian", "Italian", "Turkish"};
      
    for(int i = 0; i < 10; i++){
      std::cout << "## Building " << L[i] << " ##" << std::endl;
      Graph g1 = Graph(M[i], N[i]);
      std::cout << "Number of vertex: " << g1.get_num_vertices() << std::endl;
      std::cout << "Number of edges: " << g1.get_num_edges() << std::endl;
      g1.global_closeness_centrality();
    }
  }
  // Switching model
  Graph g2 = Graph(path);
  
  //std::cout << "Geodesic distance 1 -> 10000: " << g1.geodesic_distance(1,10000) << std::endl;
}
