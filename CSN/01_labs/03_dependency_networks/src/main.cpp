#include <iostream>
#include <fstream>
#include "graph.h"


int main() {

  std::string languages[] = {"Arabic", "Basque", "Catalan", "Chinese",
              "Czech", "English", "Greek", "Hungarian", "Italian", "Turkish"};


  const std::string DATA_DIR = "data/";
  std::string path = "";
  //const std::string path = DATA_DIR + "test.txt";

  for(std::string language : languages) {
    path = DATA_DIR + language + "_syntactic_dependency_network.txt";
    Graph g;
    std::cout << read_graph_from_file(path, g) << std::endl;
    std::cout << language <<" closeness centrality: " << calculate_cc(g) << std::endl;
  }



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
