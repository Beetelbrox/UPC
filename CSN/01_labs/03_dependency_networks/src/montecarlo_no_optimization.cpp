#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include "graph.h"

#define DEBUG true

double calculate_x_nh(const Graph &base, std::string model, std::mt19937 &eng) {
  Graph g;
  if (!model.compare("ER")) {
    generate_ER_graph(base.get_n_vertices(), base.get_n_edges(), g, eng);
  }else if (!model.compare("switching")) {
    generate_switching_graph(ceil(log2(base.get_n_edges())),base, g, eng);
  }else
    std::cerr << "Error: Bad model" << std::endl;
  return calculate_cc(g);
}


int main() {

  const std::string DATA_DIR = "data/";
  const std::string LANGUAGE_LIST_FILENAME = "language_list.txt";

  std::vector<std::string> languages;
  std::string line, langs_path = DATA_DIR + LANGUAGE_LIST_FILENAME, lang_path;

  std::random_device rd; // obtain a random number from hardware
  std::mt19937 eng(rd()); // seed the generator

  double x;

  std::ifstream f (langs_path);
  if(!f.is_open()) {
    std::cerr << "Error: Unable to open file '" << langs_path << "'" << std::endl;
    return -1;
  }

  Graph graph_lang, g2;

  lang_path = DATA_DIR + "Basque" + "_syntactic_dependency_network.txt";
  read_graph_from_file(lang_path, graph_lang);
  x = calculate_cc(graph_lang);

  int num_ER = 0, num_switching = 0, T=5, p_ER, p_switching;

  for (int i=0; i < T; ++i) {
    if (calculate_x_nh(graph_lang, "ER", eng) >= x) ++num_ER;
    if (calculate_x_nh(graph_lang, "switching", eng) >= x) ++num_switching;
  }
  p_ER = num_ER/float(T);
  p_switching = num_switching/float(T);
  std::cerr << num_ER << ", " << num_switching << std::endl;
  std::cerr << p_ER << ", " << p_switching << std::endl;

  while(getline(f, line))
    languages.push_back(line);

/*
  for (std::string lang : languages) {
    lang_path = DATA_DIR + lang + "_syntactic_dependency_network.txt";
    Graph g;  // Create a new graph object
    read_graph_from_file(lang_path, g);
    std::cout << lang << ",";
    print_summary(g);
  }
*/
}
