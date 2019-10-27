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

  int find_neighbour_ix(int id, int n_id);
public:
  Graph(); // Default constructor
  Graph( bool DEBUG ); // Debug options constructor
  // We only use default STL structures, do we need a copy constructor?

  int add_vertex();
  int add_vertex(const std::string &label);
  int add_edge(int v1, int v2);
  bool is_neighbour(int v1, int v2);

  // Getters & Setters
  std::size_t get_n_vertices() const;
  std::size_t get_n_edges() const;
  const std::vector<int>& get_neighbours(int id) const;
  int get_degree(int id) const;
  void switch_edges(int id_s1, int n_ix_1, int id_s2, int n_ix_2);
};

// Reads a sequence of pairs of vertices and returns the corresponding Graph
int read_graph_from_file(const std::string &path, Graph &g);
void generate_clique(int N, Graph &g);
void generate_disconnected_graph(int N, Graph &g);
void generate_ER_graph(int N, int E, Graph &g, std::mt19937 &eng);
void generate_switching_graph(double Q, const Graph &base, Graph &g, std::mt19937 &eng);
double calculate_cc(const Graph &g);
void print_summary(const Graph &g);

#endif
