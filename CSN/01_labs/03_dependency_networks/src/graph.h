#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <random>
#include <utility>

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
  double c_i(int id, bool pruning, double* cumm_c_i);
public:
  Graph(); // Default constructor
  Graph( bool DEBUG ); // Debug options constructor
  // We only use default STL structures, do we need a copy constructor?

  int add_vertex();
  int add_vertex(const std::string &label);
  int add_edge(int v1, int v2);
  bool is_neighbour(int v1, int v2);
  void switch_edges(std::pair<int,int> e1, std::pair<int,int> e2);
  double closeness_centrality(bool pruning=false, bool sorting=false, bool montecarlo=false, int M=0, double x=-1.0);

  // Getters & Setters
  std::size_t get_n_vertices() const;
  std::size_t get_n_edges() const;
  const std::vector<int>& get_neighbours(int id) const;
  int get_degree(int id) const;
  void get_degree_seq(std::vector<int> &deg_seq);


};

// Reads a sequence of pairs of vertices and returns the corresponding Graph
int read_graph_from_file(const std::string &path, Graph &g);
void generate_clique(int N, Graph &g);
void generate_disconnected_graph(int N, Graph &g);
void generate_ER_graph(int N, int E, Graph &g, std::mt19937 &eng);
void generate_switching_graph(double Q, const Graph &base, Graph &g, std::mt19937 &eng);
double calculate_cc(const Graph &g);
void print_summary(const Graph &g);
bool test_hypothesis_with_bounds (const Graph &g, double x);

#endif
