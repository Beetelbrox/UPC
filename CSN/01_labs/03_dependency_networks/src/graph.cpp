
#include "graph.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <algorithm>
#include <chrono>

// Use namespace std here so it is not included in the main code
using namespace std;

/* #################################################
*
* MEMBER METHODS
*
################################################# */

Graph::Graph(bool DEBUG=false): n_edges{0}, DEBUG{DEBUG}{}

Graph::Graph(const Graph &g) {
}

Graph::~Graph() {
}

// Adds a vertex to the graph indexed to the end of the list
// Returns: index
int Graph::add_vertex(const string &label){
  int id = label_dict[label];
  if (!id) {
    id = label_dict.size();
    label_dict[label] = id;
    label_index.push_back(label);
    adj_list.push_back(vector<int>());
  } else if (DEBUG)
    cerr << "Vertex with label '" << label << "' already exists with id " << id << endl;
  return id;
}

// Add unlabeled vertex to the graph.
// DO NOT MIX LABELED AND UNLABELED VERTICES, NOT CURRENTLY MANAGED
int Graph::add_vertex(){
    adj_list.push_back(vector<int>());
    return adj_list.size();
}

// This assumes that the target vertices exist and it references vertex ids directly
// The graph is undirected so we need to add it in both directions
int Graph::add_edge(int v1, int v2) {
  if (is_neighbour(v1, v2)) return -1;
  adj_list[v1-1].push_back(v2);
  adj_list[v2-1].push_back(v1);
  ++n_edges;
  return 0;
}

size_t Graph::get_n_vertices() const { return adj_list.size(); }
size_t Graph::get_n_edges() const { return n_edges; }

const std::vector<int>& Graph::get_neighbours(int id) const {
  return adj_list[id-1];
}

bool Graph::is_neighbour(int v1, int v2) {
  for (int vx : adj_list[v1-1])
    if (vx == v2) return true;
  return false;
}

double calculate_c_i(int id, const Graph &g);
void generate_clique(int N, Graph &g);

// Generate a clique (complete (sub)graph) of size N for testing purposes.
void generate_clique(int N, Graph &g) {
  cerr << "-----------------------------------------------" << endl;
  cerr << "Generating complete graph of size " << N << endl;
  cerr << "Number of generated edges: " << N*(N-1)/2.0 << endl;
  cerr << "-----------------------------------------------" << endl;
  chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
  for(int i=1; i <= N; ++i)
    g.add_vertex();
  for(int i=1; i <= N; ++i) {
    for(int j=i; j <= N; ++j)
      if (i != j) g.add_edge(i,j);
  }
  chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
  chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
  cerr << "Complete graph generated." << endl;
  cerr << "Elapsed generation time: " <<
            time_span.count() << " s" << endl;
  cerr << "-----------------------------------------------" << endl;
}

void generate_ER_graph(int N, int E, Graph &g, mt19937 &eng) {
  int placed_edges=0, v1, v2;
  // Generate integers uniformly over the interval [1,N] using the mt19937 engine
  uniform_int_distribution<int> distribution(1,N);
  cerr << "-----------------------------------------------" << endl;
  cerr << "Generating Erdos Renyi graph of size " << N << endl;
  cerr << "Number of generated edges: " << E << endl;
  cerr << "-----------------------------------------------" << endl;
  chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
  // Add the vertices into the graph
  for(int i=1; i <= N; ++i)
    g.add_vertex();
  // Add the edges to the graph. If a collision is found (loop or already existing
  // edge) discard the pair of vertices and generate a new one. Edges are generated
  // edges independently of the degree of the node, so this won't incur in any bias.
  while (placed_edges < E) {
    v1 = distribution(eng);
    v2 = distribution(eng);
    if (v1 != v2 && !g.is_neighbour(v1, v2)) {
      g.add_edge(v1, v2);
      ++placed_edges;
    }
  }
  chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
  chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
  cerr << "ER graph generated." << endl;
  cerr << "Elapsed generation time: " << time_span.count() << " s" << endl;
  cerr << "-----------------------------------------------" << endl;
}

double calculate_cc(const Graph &g) {
  double cc=0;
  // Build an index of visited nodes
  cerr << "Calculating Mean Closeness Centrality...";
  chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
  for(size_t i=1; i <= g.get_n_vertices(); ++i)
    cc += calculate_c_i(i, g);
  // Return the normalized value
  chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
  chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
  cerr << "Done." << endl;
  cerr << "Closeness Centrality: " << cc/double(g.get_n_vertices()) << endl;
  cerr << "Elapsed calculation time: " << time_span.count() << " s" << endl;
  cerr << "-----------------------------------------------" << endl;
  return cc/double(g.get_n_vertices());
}

double calculate_c_i(int id, const Graph &g) {
  // Array will store the distances. Initialized to -1 so we can check
  // if a vertex has been already visited.
  int current_vx, distances[g.get_n_vertices()];
  fill_n(distances, g.get_n_vertices(), -1);
  double c_i = 0;
  queue<int> frontier_vx;

  frontier_vx.push(id);
  distances[id-1] = 0;
  while(!frontier_vx.empty()) {
    current_vx = frontier_vx.front(); // Save the id of the next node
    frontier_vx.pop(); // Remove the node from the queue

    for (int vx : g.get_neighbours(current_vx)) {
      if (distances[vx-1] < 0){
        frontier_vx.push(vx);
        distances[vx-1] = distances[current_vx-1]+1;
        c_i += 1/double(distances[vx-1]);
      }
    }
  }
  return c_i/double(g.get_n_vertices()-1);
}

// Auxiliary function to tokenize a string to facilitate parsing.
// WARNING: Replaces all contents inside the vector 'tokens'
size_t tokenize_line(const string &line, vector<size_t> &token_pos, const string &sep=" ") {
  token_pos.clear(); // Clear the vector before pushing in the new tokens
  size_t tail = 0, head = line.find(sep);
  while ( tail < line.size() ) {
    if (head > tail) {
      token_pos.push_back(tail);
      token_pos.push_back(head-tail);
    }
    tail = head + 1;
    head = line.find(sep, tail);
    if (head > line.size()) head = line.size();
  }
  return token_pos.size()/2;
}

// This assumes that the first line in the file indicates the number of vertices
// and edges in the graph described by the file. Those numbers are used as a
// reference and not actually included in the graph.
int read_graph_from_file(const string &path, Graph &g) {
  int lines_read = 1, vx_read, exp_v, exp_e, vx[2];
  string line, w;
  vector<size_t> token_pos;
  ifstream f (path); // Create a file stream for the specified path
  if (!f.is_open()) {
    cerr << "Error: Unable to open file '" << path << "'" << endl;
    return -1;
  }
  cerr << "Reading graph from file '" << path << "'..." << endl << endl;
  getline(f, line);
  if ( tokenize_line(line, token_pos) != 2 ) {
    cerr << "Error: Malformed first row. Aborting parse" << endl;
    return -1;
  }
  cerr << "Reading header..." << endl;
  // Parse the numbers to integers and store them to check later
  exp_v = stoi( line.substr(token_pos[0], token_pos[1]) );
  exp_e = stoi( line.substr(token_pos[2], token_pos[3]) );

  cerr << "-----------------------------------------------" << endl;
  cerr << "Expected number of vertices: " << exp_v << endl;
  cerr << "Expected number of edges: " << exp_e << endl;
  cerr << "-----------------------------------------------" << endl << endl;
  cerr << "Reading edges..." << endl;
  cerr << "-----------------------------------------------" << endl;

  while(getline(f, line)) {
    ++lines_read;
    vx_read=0;
    for (size_t i=0; i < tokenize_line(line, token_pos); ++i) {
      if (!line.compare(token_pos[2*i], token_pos[2*i+1], " ")) {
        cerr << " - Bad word token found at file row " << lines_read << endl;
      } else if (++vx_read > 2){
        cerr << " - More than 2 vertices found at file row " << lines_read
              << ". Omitting additional vertices" << endl;
      } else {
        vx[i] = g.add_vertex(line.substr(token_pos[2*i], token_pos[2*i+1]));
      }
    }
    if ( vx_read != 2 ) {
      cerr << " - Malformed edge representation at file row " << lines_read << endl;
    } else if (vx[0] == vx[1]) {
      cerr << " - Loop found at file row " << lines_read << endl;
    } else
      g.add_edge(vx[0], vx[1]);
  }

  int cont = 0;
  for(size_t i=1; i <= g.get_n_vertices(); ++i) {
    cont += g.get_neighbours(i).size();
  }

  cout << cont/float(g.get_n_vertices()) << endl;

  return 0;
}
 /*

void Graph::print_adj_table() {
  cout << word_index[0] << endl;
  for(size_t i=0; i < 1; ++i) {
    cout << i+1 <<": ";
    for(size_t ix : adj_list[i]) {
      cout << ix << " ";
    }
    cout << endl;
  }
}


int Graph::geodesic_distance(int ix_s, int ix_d){
  return 0;
}

Graph& read_graph_from_file(std::string path) {

}
*/
