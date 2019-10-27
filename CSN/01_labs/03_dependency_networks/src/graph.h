#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <unordered_map>
#include <random>

class Graph {
private:
  // The adjacency list cointains edges in both directions to allow
  // for fast indexing. Could be done with two lists. Faster?
  // Implemented as a list of lists because the average degree is very low;
  std::vector<std::vector<int>> adj_list;
  std::vector<std::string> label_index;      // Allows to access word values by index
  std::unordered_map<std::string, int> label_dict;
  size_t n_edges;
  bool DEBUG;
public:
  Graph(bool DEBUG); // Default constructor
  ~Graph();
  Graph(const Graph &g);  // Copy constructor

  int add_vertex();
  int add_vertex(const std::string &label);
  int add_edge(int v1, int v2);
  bool is_neighbour(int v1, int v2);

  // Getters & Setters
  std::size_t get_n_vertices() const;
  std::size_t get_n_edges() const;
  const std::vector<int>& get_neighbours(int id) const;

  //const std::vector<int>& get_neighbours(int id);


};

// Reads a sequence of pairs of vertices and returns the corresponding Graph
int read_graph_from_file(const std::string &path, Graph &g);
void generate_clique(int N, Graph &g);
void generate_ER_graph(int N, int E, Graph &g, std::mt19937 &eng);
double calculate_cc(const Graph &g);

#endif
