#include <string>
#include <vector>

class Graph {
private:
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

  int geodesic_distance(int ix_s, int ix_d);

  Graph* test(){return this;}


  // Getters & Setters
  const std::size_t get_n_vertices();
  const std::size_t get_n_edges();
  const std::string get_word_by_id(const std::size_t id);

  //const std::vector<int>& get_neighbours(int id);


};
