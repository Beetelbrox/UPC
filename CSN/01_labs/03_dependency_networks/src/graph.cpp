#include "graph.h"
#include <fstream>
#include <iostream>
#include <unordered_map>

// https://stackoverflow.com/questions/2041517/random-simple-connected-graph-generation-with-given-sparseness

int find_or_add(const std::string &w,
  std::unordered_map<std::string, int> &word_dict,
  std::vector<std::string> &word_index,
  std::vector<std::vector<int>> &adj_list);

Graph::Graph() {

}

Graph::Graph(const Graph &g) {

}

Graph::~Graph() {

}

const std::string Graph::get_word_by_id(int id) {
  return ((unsigned int)id < adj_list.size() && id > 0) ? word_index[id-1] : "";
}

const int Graph::get_num_vertices(){ return adj_list.size(); }

const std::vector<int>& Graph::get_neighbours(int id) { return adj_list[id-1]; }

/*
*
* STATIC METHODS
*
*/

Graph Graph::from_file(const std::string &path) {
  std::ifstream f (path);
  int num_nodes, num_edges, o_ix, d_ix;
  std::string orig, dest;
  std::unordered_map<std::string, int> word_dict;
  Graph g;
  // Sanity check to avoid trying to read from a clsed file

  if ( f.is_open() ) {
    f >> num_nodes >> num_edges;
    while(!f.eof()) {
      f >> orig >> dest;
      o_ix = find_or_add(orig, word_dict, g.word_index, g.adj_list);
      d_ix = find_or_add(dest, word_dict, g.word_index, g.adj_list);
      g.adj_list[o_ix-1].push_back(d_ix);
    }
  }
  return g;
}

int Graph::geodesic_distance(int ix_s, int ix_d){
  return 0;
}


// Helper Private methods
int find_or_add(const std::string &w,
                std::unordered_map<std::string, int> &word_dict,
                std::vector<std::string> &word_index,
                std::vector<std::vector<int>> &adj_list) {
  int w_ix = word_dict[w];
  if (!w_ix) {
    word_index.push_back(w);
    adj_list.push_back(std::vector<int>());
    word_dict[w] = word_index.size();
    w_ix = word_index.size();
  }
  return w_ix;
}
