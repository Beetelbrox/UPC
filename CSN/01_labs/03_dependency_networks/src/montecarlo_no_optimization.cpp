#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include "graph.h"

#define DEBUG true

/*
double calculate_x_nh(const Graph &base, std::string model, std::mt19937 &eng) {
  Graph g;
  if (!model.compare("ER")) {
    generate_ER_graph(base.get_n_vertices(), base.get_n_edges(), g, eng);
  }else if (!model.compare("switching")) {
    generate_switching_graph(ceil(log2(base.get_n_edges())),base, g, eng);
  }else
    std::cerr << "Error: Bad model" << std::endl;
  return calculate_cc(g);
}*/


int main() {

  //double ccs[] = {0.32646, 0.269719, 0.34101, 0.326454, 0.305948, 0.343509, 0.314717, 0.288346, 0.32782, 0.360334};

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

/*

  lang_path = DATA_DIR + "Czech" + "_syntactic_dependency_network.txt";
  //lang_path = DATA_DIR + "test.txt";
  read_graph_from_file(lang_path, graph_lang);
  x = graph_lang.closeness_centrality(true, true, true, graph_lang.get_n_vertices()/10);
  */

  //int num_ER = 0, num_switching = 0, T=5, p_ER, p_switching;

/*
  for (int i=0; i < T; ++i) {
    if (calculate_x_nh(graph_lang, "ER", eng) >= x) ++num_ER;
    if (calculate_x_nh(graph_lang, "switching", eng) >= x) ++num_switching;
  }
  p_ER = num_ER/float(T);
  p_switching = num_switching/float(T);
  std::cerr << num_ER << ", " << num_switching << std::endl;
  std::cerr << p_ER << ", " << p_switching << std::endl;
x = graph_lang.closeness_centrality(true, true);
generate_ER_graph(graph_lang.get_n_vertices(), graph_lang.get_n_edges(), g2, eng);
g2.closeness_centrality(true, true);
generate_switching_graph(ceil(log2(graph_lang.get_n_edges())),graph_lang, g3, eng);
g3.closeness_centrality(true, true);

*/
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
