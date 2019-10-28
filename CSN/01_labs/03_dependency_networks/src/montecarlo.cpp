#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include "graph.h"

#define DEBUG true
using namespace std;

int main() {

  double ccs[] = {0.32646, 0.269719, 0.34101, 0.326454, 0.305948, 0.343509, 0.314717, 0.288346, 0.32782, 0.360334};

  const string DATA_DIR = "data/";
  const string LANGUAGE_LIST_FILENAME = "language_list.txt";

  vector<string> languages;
  string line, langs_path = DATA_DIR + LANGUAGE_LIST_FILENAME, lang_path;

  random_device rd; // obtain a random number from hardware
  mt19937 eng(rd()); // seed the generator

  int T = 20;
  double percentage = 0.05;

  ifstream f (langs_path);
  if(!f.is_open()) {
    cerr << "Error: Unable to open file '" << langs_path << "'" << endl;
    return -1;
  }
  int j = 0;
//   lang_path = DATA_DIR + "Arabic" + "_syntactic_dependency_network.txt";
//   Graph g_lang;
//   read_graph_from_file(lang_path, g_lang);
//   x = g_lang.closeness_centrality(true, true);

  while(getline(f, line))
    languages.push_back(line);
  cout << "Start Montecarlo, T = 20, 10% of nodes: " << endl;
  for (string lang : languages) {    
    lang_path = DATA_DIR + lang + "_syntactic_dependency_network.txt";
    int i = 0;
    double accumulator_over_experiment = 0.0;
    while (i < T){
        Graph g_lang, g_ER, g_sw;  // Create a new graph object
        read_graph_from_file(lang_path, g_lang);
        int vertex_number = g_lang.get_n_vertices();
        accumulator_over_experiment += g_lang.closeness_centrality(true, false, -1, vertex_number*percentage);
        i++;
    }
    cout << "\nFinal average Mean Closeness Centrality: " << accumulator_over_experiment/double(T) << endl;
    cout << "Real :" << ccs[j] << endl;
    cout << "\n" << endl;
    j++;
  }
}