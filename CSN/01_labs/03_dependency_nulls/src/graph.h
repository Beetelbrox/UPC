#include <string>
#include <vector>

class Graph {
  private:
    std::vector<std::vector<int>> adj_list;
    int nNodes;
    const int BFS(int source, int dest);
    int geodesic_distance(int ix_s, int ix_d);
  public:
    Graph(int NoE, int NoV); // Default constructor
    ~Graph();
    Graph(const Graph &g);

    // Getters & Setters
    const int get_num_vertices();
    const int get_num_edges();
    void GenerateGraph(int NoE, int NoV);
    void closeness_centrality(int s);

};
