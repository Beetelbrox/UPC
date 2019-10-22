#include <string>
#include <vector>

class Graph {
private:
  std::vector<std::vector<int>> adj_list;
  std::vector<std::string> word_index;
public:
  Graph(); // Default constructor
  ~Graph();
  Graph(const Graph &g);

  int add_vertex(const std::string label);

  static Graph from_file(const std::string &path);

  int geodesic_distance(int ix_s, int ix_d);

  Graph* test(){return this;}


  // Getters & Setters
  const std::string get_word_by_id(int id);
  const int get_num_vertices();
  const int get_num_edges();

  const std::vector<int>& get_neighbours(int id);


};
