#include "graph.h"

Graph::Graph (const uint &N) {
    adjacency = Matrix(N, Vector());
}

uint Graph::create_node () {
    adjacency.push_back(Vector());
    return adjacency.size()-1;
}

void Graph::add_edge (const uint &u, const uint &v) {
    adjacency[u].push_back(v);
    adjacency[v].push_back(u);
}

uint Graph::n_vertices () const {
    return adjacency.size();
}

uint Graph::degree (const uint &node) const {
    return adjacency[node].size();
}

bool Graph::has_connection (const uint &u, const uint &v) const {
    if (u == v)
        return true;
    if (adjacency[u].size() > adjacency[v].size())
        return find(adjacency[v].begin(), adjacency[v].end(), u) != adjacency[v].end();
    else
        return find(adjacency[u].begin(), adjacency[u].end(), v) != adjacency[u].end();
}

void Graph::write_degree_sequence (ostream &output) const {
    for (uint i = 0; i < adjacency.size(); ++i)
        output << degree(i) << endl;
}

void Graph::write_limited_degree_sequence (uint lim, ostream &output) const {
    lim = min(lim, (uint) adjacency.size());
    for (uint i = 0; i < lim; ++i)
        output << degree(i) << endl;
}