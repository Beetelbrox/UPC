#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include "graph.h"

#define DEBUG true

int main() {

  const std::string DATA_DIR = "data/";
  const std::string LANGUAGE_LIST_FILENAME = "language_list.txt";

  std::vector<std::string> languages;
  std::string line, langs_path = DATA_DIR + LANGUAGE_LIST_FILENAME, lang_path;

  std::random_device rd; // obtain a random number from hardware
  std::mt19937 eng(rd()); // seed the generator

  std::ifstream f (langs_path);
  if(!f.is_open()) {
    std::cerr << "Error: Unable to open file '" << langs_path << "'" << std::endl;
    return -1;
  }
  while(getline(f, line))
    languages.push_back(line);

  double cc_x;
  int cont_ER, cont_sw;


  for (std::string lang : languages) {
    cont_ER=0; cont_sw=0;
    lang_path = DATA_DIR + lang + "_syntactic_dependency_network.txt";
    Graph g_lang;  // Create a new graph object
    read_graph_from_file(lang_path, g_lang);
    int M = g_lang.get_n_vertices()/50, reps=25;
    cc_x = g_lang.closeness_centrality(true, true, true, M);
    for(int i=0; i < reps; ++i) {
      Graph g_ER, g_sw;
      std::cerr << "Iteration " << i+1 << std::endl;
      generate_ER_graph(g_lang.get_n_vertices(), g_lang.get_n_edges(), g_ER, eng);
      if (g_ER.closeness_centrality(true, true, true, M) >= cc_x) ++cont_ER;
      generate_switching_graph(ceil(log2(g_lang.get_n_edges())),g_lang, g_sw, eng);
      if(g_sw.closeness_centrality(true, true, true, M) >= cc_x) ++ cont_sw;
    }
    std::cout << cc_x << " " << cont_ER/double(reps) << " " << cont_sw/double(reps) << std::endl;
  }

}
