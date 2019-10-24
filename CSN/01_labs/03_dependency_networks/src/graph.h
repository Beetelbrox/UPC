#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>

class Graph {
private:
  // The adjacency list cointains edges in both directions to allow
  // for fast indexing. Could be done with two lists. Faster?
  std::vector<std::vector<std::size_t>> adj_list;
  std::vector<std::string> word_index;      // Allows to access word values by index
  size_t n_edges;
public:
  Graph(); // Default constructor
  ~Graph();
  Graph(const Graph &g);

  size_t add_vertex(const std::string &label);
  void add_edge(const std::size_t v1, const std::size_t v2);

  static Graph from_file(const std::string &path);

  unsigned int calculate_c_i(std::size_t id);

  int geodesic_distance(int ix_s, int ix_d);

  // Getters & Setters
  const std::size_t get_n_vertices();
  const std::size_t get_n_edges();

  void print_adj_table();

  //const std::vector<int>& get_neighbours(int id);


};

// Reads a sequence of pairs of vertices and returns the corresponding Graph
Graph& read_graph_from_file(std::string path);

#endif
