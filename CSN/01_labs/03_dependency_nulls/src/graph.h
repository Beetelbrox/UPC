#include <string>
#include <vector>

class Graph {
  private:
    std::vector<std::vector<int>> adj_list;
    int nNodes;
    const double closeness_centrality_single(int source);
    void print_adj(int value);
    // int geodesic_distance(int ix_s, int ix_d);
  public:
    Graph(int NoE, int NoV); // Default constructor
    Graph(std::string path);
    ~Graph();
    Graph(const Graph &g);

    // Getters & Setters
    const int get_num_vertices();
    const int get_num_edges();
    void GenerateGraph(int NoE, int NoV);
    void global_closeness_centrality();
    void read_graph_from_file(std::string path);   
    void apply_switching(int Q_value);
};
