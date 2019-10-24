
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <queue>

#include "graph.h"

// Use namespace std here so it is not included in the main code
using namespace std;

size_t find_vertex(const string &label, const unordered_map<string, size_t> &word_dict) {
  if( word_dict.find(label) == word_dict.end() ) return 0;
  else return word_dict.find(label) -> second;
}

/*
* #################################################
* MEMBER METHODS
* #################################################
*/

Graph::Graph() {
  n_edges=0;

}

Graph::Graph(const Graph &g) {
}

Graph::~Graph() {
}

// Adds a vertex to the graph indexed to the end of the list
// Returns: index
size_t Graph::add_vertex(const string &label){
  word_index.push_back(label);
  adj_list.push_back(vector<size_t>());
  return word_index.size();
}

// This assumes that the target vertices exist and it references vertex ids directly
// The graph is undirected so we need to add it in both directions
void Graph::add_edge(const std::size_t v1, const std::size_t v2) {
  cout << "Adding edge (" << v1 << ", " << v2 << ")" << endl;
  adj_list[v1-1].push_back(v2);
  adj_list[v2-1].push_back(v1);
  ++n_edges;
}

const size_t Graph::get_n_vertices(){ return adj_list.size(); }
const size_t Graph::get_n_edges() { return n_edges; }

unsigned int Graph::calculate_c_i(size_t id) {
  size_t count=1, current;
  queue<size_t> frontier_vertices;
  bool visited[adj_list.size()] = {};  // Zero-initialized array


  frontier_vertices.push(id);
  visited[id-1] = true;
  while(frontier_vertices.size() > 0) {
    current = frontier_vertices.front();
    frontier_vertices.pop();
    //cout << "Exploring vertex " << current << "..." << endl;
    for (size_t vx : adj_list[current-1]) {
      if (!visited[vx-1]){
        //cout << "Vertex " << vx << " found." << endl;
        frontier_vertices.push(vx);
        visited[vx-1] = true;
        ++count;
      } else {
        //cout << "Vertex " << vx << " already visited." << endl;
      }
    }
    //cout << endl;
  }
  cout << count << endl;
  return 0;
}

/*
*
* STATIC METHODS
*
*/

// This is slightly elaborated to address for weird stuff/inbalances in
// the graph input data. Want to ensure correctness when reading.
Graph Graph::from_file(const std::string &path) {
  size_t lines_read = 0, skipped=0, found, n_vertices, n_edges;
  string line, t1, t2;

  Graph g;
  ifstream f (path);
  unordered_map<string, size_t> word_dict;  // Allows for very fast check of existence

  // Sanity check to avoid trying to read from a clsed file
  // Check
  if ( f.is_open() ) {
    // Read the first line and parse the (expected) number of vertices and edges
    getline(f, line);
    found = line.find(' ');
    n_vertices = stoi(line.substr(0, found));
    n_edges = stoi(line.substr(found+1));

    while(getline(f, line)) {
      ++lines_read;
      found = line.find(' ');
      t1 = line.substr(0, found);
      t2 = line.substr(found+1);
      // Ad-hoc battery of checks for malformed inputs
      if (!t1.size() ||
          !t2.size() ||
          !t1.compare(" ") ||
          !t2.compare(" ") ||
          !t1.compare(" ") || // The last two are non-breaking spaces that appear
          !t2.compare(" ") )  // In some of the datasets (kudos to Amalia for noticing)
          {
        cerr << "Line " << lines_read << ": Malformed input (" << line << "). Skipping line." << endl;
        ++skipped;
      } else if (!t1.compare(t2)){
        cerr << "Line " << lines_read << ": Loop found (" << line << "). Skipping line." << endl;
        ++skipped;
      } else {
        if(!find_vertex(t1, word_dict)) word_dict[t1] = g.add_vertex(t1);
        if(!find_vertex(t2, word_dict)) word_dict[t2] = g.add_vertex(t2);
        g.add_edge(find_vertex(t1, word_dict), find_vertex(t2, word_dict));
      }
    }
  }
  cout << "Skipped lines: " << skipped << endl;
  cout << "Num vertices expected: " << n_vertices << endl;
  cout << "Num vertices read: " << g.get_n_vertices() << endl;
  cout << "Num edges expected: " << n_edges << endl;
  cout << "Num edges read: " << g.get_n_edges() << endl;
  return g;
}

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
