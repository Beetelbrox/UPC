#include <iostream>
#include <fstream>
#include <vector>
#include "graph.h"

#define DEBUG true


int main() {
  const std::string DATA_DIR = "data/";
  const std::string LANGUAGE_LIST_FILENAME = "language_list.txt";

  std::vector<std::string> languages;
  std::string line, langs_path = DATA_DIR + LANGUAGE_LIST_FILENAME, lang_path;

  std::ifstream f (langs_path);
  if(!f.is_open()) {
    std::cerr << "Error: Unable to open file '" << langs_path << "'" << std::endl;
    return -1;
  }

  while(getline(f, line)) {
    languages.push_back(line);
  }

  for (std::string lang : languages) {
    lang_path = DATA_DIR + lang + "_syntactic_dependency_network.txt";
    Graph g;  // Create a new graph object
    read_graph_from_file(lang_path, g);
    std::cout << lang << ",";
    print_summary(g);
  }


/*
  for(std::string language : languages) {
    path = DATA_DIR + language + "_syntactic_dependency_network.txt";
    Graph g;
    std::cout << read_graph_from_file(path, g) << std::endl;
    std::cout << language <<" closeness centrality: " << calculate_cc(g) << std::endl;
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
