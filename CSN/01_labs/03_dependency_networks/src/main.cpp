#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "graph.h"


int main() {


  const std::string DATA_DIR = "data/";
  const std::string path = DATA_DIR + "Basque_syntactic_dependency_network.txt";
  //const std::string path = DATA_DIR + "test.txt";

  Graph g = Graph::from_file(path);
  //g.print_adj_table();
  for(std::size_t i=1; i <= g.get_n_vertices(); ++i) {
    //g.calculate_c_i(i);
  }
  g.calculate_c_i(1);

  /*
  int count = 0;
  for(int i=1; i <= 3934; ++i) {
    std::cout << i << " " << g.get_word_by_id(i) << std::endl;
  }
  */

  // To load the graph, if we want to do Numeric:
  // 1. Read all words and sort them, that will be the indexpyth

  // If we process the sequence of vertices on the first pass finding the index
  // is linear?
  // We can have a dictionary indicating what have we sorted so far.
  // Structures for the processing:
  //  - Vector of integers representing the adjacency list
  //  - Array containing the index of words
  //  - Dictionary (Hashmap?) to list what have we read so far and the corresponding
  //  - index to be able to find things fast without need for a random reordering
  //    at the end.
}
