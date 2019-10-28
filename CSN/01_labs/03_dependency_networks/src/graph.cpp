
#include "graph.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <algorithm>
#include <chrono>

// Use namespace std here so it is not included in the main code
using namespace std;

#define UPPER_BOUND (double c_m, int N, int M) ( 1 + (c_m)/double(N) - (M)/float(N) )
#define LOWER_BOUND (double c_m, int N) ((c_m)/float(N)

/* #################################################
*
* MEMBER METHODS
*
################################################# */

Graph::Graph(): n_edges{0}, DEBUG{false}{}
Graph::Graph(bool DEBUG): n_edges{0}, DEBUG{DEBUG}{}

// Add unlabeled vertex to the graph.
// DO NOT MIX LABELED AND UNLABELED VERTICES, NOT CURRENTLY MANAGED
int Graph::add_vertex(){
    adj_list.push_back(vector<int>());
    return adj_list.size();
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
const std::vector<int>& Graph::get_neighbours(int id) const { return adj_list[id-1]; }
int Graph::get_degree(int id) const { return adj_list[id-1].size(); }

bool Graph::is_neighbour(int v1, int v2) {
  for (int vx : adj_list[v1-1])
    if (vx == v2) return true;
  return false;
}

// This returns a 0-based index
int Graph::find_neighbour_ix(int id, int n_id) {
  for (size_t i=0; i < adj_list[id-1].size(); ++i)
    if(adj_list[id-1][i] == n_id) return i;
  return -1;
}

void Graph::switch_edges(pair<int,int> e1, pair<int,int> e2) {
  // Save the intial values stored in the adjacency lists
  int n1 = adj_list[e1.first-1][e1.second];
  int n2 = adj_list[e2.first-1][e2.second];
  // Swap the values in the first direction
  iter_swap(adj_list[e1.first-1].begin()+e1.second, adj_list[e2.first-1].begin()+e2.second);
  // Swap the values in the second direction by finding their positions in the adjacency list
  iter_swap(find(adj_list[n1-1].begin(), adj_list[n1-1].end(), e1.first), find(adj_list[n2-1].begin(), adj_list[n2-1].end(), e2.first) );
}

void Graph::get_degree_seq(vector<int> &deg_seq) {
  deg_seq.clear();
  for(vector<int> v : adj_list)
    deg_seq.push_back(v.size());
}

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

void generate_disconnected_graph(int N, Graph &g) {
  cerr << "-----------------------------------------------" << endl;
  cerr << "Generating disconnected graph of size " << N << endl;
  cerr << "-----------------------------------------------" << endl;
  chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
  for(int i=1; i <= N; ++i)
    g.add_vertex();
  chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
  chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
  cerr << "Disconnected graph generated." << endl;
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

int choose_edge_uar(const Graph &g, pair<int, int> &e, std::mt19937 &eng) {
  uniform_int_distribution<int> dist_vx(1, g.get_n_vertices()), dist_neighbours;
  e.first = dist_vx(eng);
  if (!g.get_degree(e.first)) return -1;
  dist_neighbours = uniform_int_distribution<int>(0,g.get_degree(e.first)-1);
  e.second = dist_neighbours(eng); // Second value of the pair is the index on the neighbour list
  return 0;
}

void generate_switching_graph(double Q, const Graph &base, Graph &g, std::mt19937 &eng){
  int n1, n2, cont=0;
  pair<int, int> e1, e2;

  cerr << "-----------------------------------------------" << endl;
  cerr << "Generating switching graph of size " << base.get_n_vertices() << endl;
  cerr << "Number of edges: " << base.get_n_edges() << endl;
  cerr << "Number of planned switches: " << Q*base.get_n_edges() << endl;
  cerr << "-----------------------------------------------" << endl;
  chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();

  g = base;
  for(int i=0; i < base.get_n_edges()*Q; ++i) {
    // Choose two edges uniformly at random. Discard if some of the vertices chosen is inconnected
    if (!choose_edge_uar(g, e1, eng) && !choose_edge_uar(g, e2, eng)) {
      // Obtain the indices for the destination vertices
      n1 = g.get_neighbours(e1.first)[e1.second];
      n2 = g.get_neighbours(e2.first)[e2.second];
      // Check if it is a valid swap
      if (!(g.is_neighbour(e1.first, n2) || g.is_neighbour(e2.first, n1) || // multiedges
        e1.first == n2 || e2.first == n1 || // Loops
        e1.first == e2.first || n1 == n2 )){ // Trivial swaps
          g.switch_edges(e1, e2);
          ++cont;
        }
    }
  }
  chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
  chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
  cerr << "Switching graph generated." << endl;
  cerr << "Switches performed: " << cont << endl;
  cerr << "Collisions found: " << Q*base.get_n_edges()-cont << endl;
  cerr << "Switch failure rate: " << (Q*base.get_n_edges()-cont)/float(Q*base.get_n_edges())*100 << "%" <<endl;
  cerr << "Elapsed generation time: " << time_span.count() << " s" << endl;
  cerr << "-----------------------------------------------" << endl;
}

inline double upper_bound (double c_m, int N, int M) { return 1 + c_m/double(N) - M/float(N); }
inline double lower_bound (double c_m, int N) { return c_m/float(N); }

double Graph::closeness_centrality(bool pruning, bool sorting, double x) {
  int visited[adj_list.size()], degree_oners, cur_vx, M=0, hit_bound=0;
  fill_n(visited, adj_list.size(), 0);
  double cummulator = 0.0, c_i_plus_1, lb, ub;
  vector<pair<int, int>> ordering;

  cerr << "Calculating Mean Closeness Centrality..." << endl;
  if (pruning) cerr << "Neighbour optimization enabled" << endl;
  if (sorting) cerr << "Vertex sorting enabled" << endl;
  chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();

  if (sorting) {
    for (size_t i=1; i <= adj_list.size(); ++i) ordering.push_back(make_pair(adj_list[i-1].size(), i));
    sort(ordering.begin(), ordering.end(), greater<>());
  }
  for(size_t i=1; i <= adj_list.size() && !hit_bound; ++i) {
    cur_vx = sorting ? ordering[i-1].second : i;
    if (!visited[cur_vx-1]) {
      visited[cur_vx-1] = 1;
      cummulator += c_i(cur_vx, pruning, &c_i_plus_1);
      ++M;
      if (pruning) {
        degree_oners=0;
        for (int nbr : adj_list[cur_vx-1]) {
          if (adj_list[nbr-1].size() == 1 && !visited[nbr-1]) {
            ++degree_oners;
            visited[nbr-1] = 1;
          }
        }
        cummulator += degree_oners*c_i_plus_1;
        M += degree_oners;
      }
    }
    if (x >= 0) {
      ub = upper_bound(cummulator, adj_list.size(), M);
      lb = lower_bound(cummulator, adj_list.size());
      if (ub < x) {
        cerr << "Upper bound " << ub << " smaller than x=" << x <<endl;
        hit_bound = 1;
      } else if (lb >= x) {
        cerr << "Lower bound " << ub << " greater or equal than x=" << x <<endl;
        hit_bound = -1;
      }
    }
  }
  chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
  chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
  cerr << "Done." << endl;
  cerr << "Closeness Centrality: " << cummulator/double(adj_list.size()) << endl;
  cerr << "Elapsed calculation time: " << time_span.count() << " s" << endl;
  cerr << "-----------------------------------------------" << endl;
  return cummulator/double(adj_list.size());
}

double Graph::c_i(int id, bool pruning, double* cumm_c_i_1) {
  int cur_vx, distances[adj_list.size()];
  fill_n(distances, adj_list.size(), -1);
  double cummulator = 0.0;
  queue<int> frontier_vx;

  frontier_vx.push(id);   // Push the starting node into the queue
  distances[id-1] = 0;    // Set the initial distance to 0
  *cumm_c_i_1 = 0;        // Initialize the +1 dist cummulator to 0
  while(!frontier_vx.empty()) {
    cur_vx = frontier_vx.front();   // Get the first element in the queue
    frontier_vx.pop();                  // Pop it
    for (int vx : adj_list[cur_vx-1]) {
      if(distances[vx-1] < 0) {
        frontier_vx.push(vx);
        distances[vx-1] = distances[cur_vx-1]+1;
        cummulator += 1/double(distances[vx-1]);
        if (pruning) *cumm_c_i_1 += 1/double(distances[vx-1] + 1);
      }
    }
  }
  // Add 1 to account for the distance to vertex i and substract 0.5 for the neighbour
  if (pruning) *cumm_c_i_1 = (*cumm_c_i_1 + 0.5)/double(adj_list.size()-1);
  return cummulator/double(adj_list.size()-1);
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
  int lines_read = 1, vx_read, exp_v, exp_e, vx[2], loops=0, multiedges=0;
  string line, w;
  vector<size_t> token_pos;
  ifstream f (path); // Create a file stream for the specified path
  if (!f.is_open()) {
    cerr << "Error: Unable to open file '" << path << "'" << endl;
    return -1;
  }
  cerr << "***********************************************" << endl;
  cerr << "Reading graph from file '" << path << "'..." << endl;
  getline(f, line);
  if ( tokenize_line(line, token_pos) != 2 ) {
    cerr << "Error: Malformed first row. Aborting parse" << endl;
    return -1;
  }
  // Parse the numbers to integers and store them to check later
  exp_v = stoi( line.substr(token_pos[0], token_pos[1]) );
  exp_e = stoi( line.substr(token_pos[2], token_pos[3]) );

  cerr << "-----------------------------------------------" << endl;
  cerr << " - Expected number of vertices: " << exp_v << endl;
  cerr << " - Expected number of edges: " << exp_e << endl;

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
      ++loops;
    } else if (g.add_edge(vx[0], vx[1]))
      ++multiedges;
  }
  cerr << "-----------------------------------------------" << endl;
  cerr << "Graph succesfully read!" << endl;
  cerr << " - Vertices succesfully read: " << g.get_n_vertices() << endl;
  cerr << " - Edges succesfully read: " << g.get_n_edges() << endl;
  cerr << " - Loops omitted: " << loops << endl;
  cerr << " - Duplicate edges omitted: " << multiedges << endl;
  cerr << "***********************************************" << endl << endl;
  return 0;
}

void print_summary(const Graph &g) {
  int N = g.get_n_vertices(), E = g.get_n_edges();
  cout << N << ","
       << E << ","
       << 2*E /float(N) << ","
       << 2*E/float(N*(N-1))
       << endl;
}
