#include <string>
#include <vector>

class Graph {
  private:
    std::vector<std::vector<int>> adj_list;
  public:
    Graph(int NoE, int NoV); // Default constructor
    ~Graph();
    Graph(const Graph &g);

    int geodesic_distance(int ix_s, int ix_d);

    // Getters & Setters
    const int get_num_vertices();
    const int get_num_edges();
    void GenerateGraph(int NoE, int NoV);
    const int BFS(int source, int dest, int n, int pred[], int dist[]);

};
