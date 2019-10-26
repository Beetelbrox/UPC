#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <unordered_map>

class Graph {
private:
  // The adjacency list cointains edges in both directions to allow
  // for fast indexing. Could be done with two lists. Faster?
  std::vector<std::vector<int>> adj_list;
  std::vector<std::string> label_index;      // Allows to access word values by index
  std::unordered_map<std::string, int> label_dict;
  size_t n_edges;
public:
  Graph(); // Default constructor
  ~Graph();
  Graph(const Graph &g);  // Copy constructor

  int add_vertex(const std::string &label);
  int add_edge(const int v1, const int v2);

  // Getters & Setters
  std::size_t get_n_vertices() const;
  std::size_t get_n_edges() const;
  const std::vector<int>& get_neighbours(int id) const;

  //const std::vector<int>& get_neighbours(int id);


};

// Reads a sequence of pairs of vertices and returns the corresponding Graph
int read_graph_from_file(const std::string &path, Graph &g);
double calculate_cc(const Graph &g);

#endif
