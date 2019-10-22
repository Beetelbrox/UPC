#include <string.h> // For strtok
#include "graph.h"
#include <fstream>
#include <iostream>
#include <unordered_map>

// https://stackoverflow.com/questions/2041517/random-simple-connected-graph-generation-with-given-sparseness

int find_or_add(const std::string &w,
  std::unordered_map<std::string, int> &word_dict,
  std::vector<std::string> &word_index,
  std::vector<std::vector<int>> &adj_list);

/*
* #################################################
* MEMBER METHODS
* #################################################
*/

Graph::Graph() {
    std::cout << "Constructor: " << this << std::endl;

}

Graph::Graph(const Graph &g) {
  std::cout << "I am the copy constructor" << std::endl;
}

Graph::~Graph() {
  std::cout << "I am being destroyed: " << this << std::endl;
}

// Adds a vertex to the graph indexed to the end of the list
// Returns: index
int Graph::add_vertex(const std::string label){
  return 0;
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

// This is slightly elaborated to address for weird stuff/inbalances in
// the graph input data. Want to ensure correctness when reading.
Graph Graph::from_file(const std::string &path) {
  int count=0, num_nodes, num_edges, o_ix, d_ix;
  std::string line, token, orig, dest;
  std::unordered_map<std::string, int> word_dict;
  Graph g;

  std::ifstream file(path);

  // Sanity check to avoid trying to read from a clsed file
  // Check
  if ( file.is_open() ) {
    // Regular operator don't consume the endline
    std::getline(file, line);
    std::cout << line.substr(0, line.find(" ")) << std::endl;
    int ptr;
    while(std::getline(file, line)) {

    }
    //file >> num_nodes >> num_edges; // Read the number of nodes and edges.

/*
    file >> token;  // Read the first one so the to manage possible newlines at
    // the end of the file
    while(!file.eof()){
      o_ix = find_or_add(token, word_dict, g.word_index, g.adj_list);
      file >> token;
      d_ix = find_or_add(token, word_dict, g.word_index, g.adj_list);
      g.adj_list[o_ix-1].push_back(d_ix);
      ++count;
      file >> token;

    }

  }
  */
  //std::cout << num_edges << " " << count << std::endl;
  return g;
} }

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
