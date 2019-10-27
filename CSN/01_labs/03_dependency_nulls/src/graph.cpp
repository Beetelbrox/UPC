#include "graph.h"
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <bits/stdc++.h> 
#include <algorithm>

using namespace std;

struct greatert
{
    template<class T>
    bool operator()(T const &a, T const &b) const { return a > b; }
};

// Main constructor takes 
Graph::Graph(int NoE, int NoV) {
  cout << "Building Graph" << endl;
  nNodes = NoV;
  clock_t begin = clock();
  GenerateGraph(NoE, NoV);
  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  cout << "Constructor " << this << " finished in " << elapsed_secs << " seconds." << endl;
}

Graph::Graph(string path) {
  
  read_graph_from_file(path);
  cout << "Building Graph" << endl;
  nNodes = get_num_vertices();
  cout << "Num Vertex: " << nNodes << endl;
  cout << "Num Edges: " << get_num_edges() << endl;
 
  vector<int> sequence_pre;
  vector<int> sequence_post;

  for(int i = 0; i < nNodes; i++){
    sequence_pre.push_back(adj_list[i].size());
  }
  
  apply_switching(10);

  for(int i = 0; i < nNodes; i++){
    sequence_post.push_back(adj_list[i].size());
  }
  sort(sequence_pre.begin(), sequence_pre.end());
  sort(sequence_post.begin(), sequence_post.end());
  
  
  for(int i = 0; i < nNodes; i++){
    if (sequence_pre[i] != sequence_post[i]){
      cout << "Not same sequence!" << endl;
      return;
    }
  }

}

void Graph::print_adj(int value){
  cout << value << " -> {";
  for (int j = 0; j < adj_list[value].size(); j++){
     cout << " " << adj_list[value].at(j);
  }
  cout << " }" << endl;
}

void Graph::apply_switching(int Q_value){
  int nEdges = get_num_edges();
  int nVertex = get_num_vertices();
  int notvalid_count = 0;
  int notallowed_count = 0;
  int chosen_u = 0;
  int chosen_v = 0;
  int chosen_s = 0;
  int chosen_t = 0;
  int index_u = 0;
  int index_v = 0; 
  int index_s = 0; 
  int index_t = 0;
  cout << "Q Value : " << Q_value << endl;  
  int number_switches = Q_value * nEdges;
  //number_switches = 10;
  cout << "Number of switches " << number_switches << endl;
  int i = 0;
  while (i < number_switches){
    
    // First pair
    chosen_u = rand()%nVertex;     
    // Test Isolated   
    if (adj_list[chosen_u].size() == 0) {
      notvalid_count++;
      i++;
      continue;
    }
    index_v = rand()%adj_list[chosen_u].size();
    chosen_v = adj_list[chosen_u].at(index_v);
    // Second pair
    chosen_s = rand()%nVertex;
    // Test Isolated
    if (adj_list[chosen_s].size() == 0) {
      notvalid_count++;
      i++;
      continue;
    }
    // Test if same, then trivial
    if (chosen_u == chosen_s){
      cout << "Trivial?" << endl;
      notvalid_count++;
      i++;
      continue;
    }
    index_t = rand()%adj_list[chosen_s].size();
    chosen_t = adj_list[chosen_s].at(index_t);
    // A self reference (loop) will happen
    if (chosen_t == chosen_u){
      cout << "Loop avoided {" << chosen_u << "," << chosen_v << "},{" << chosen_s << "," << chosen_t << "}" << endl;
      notallowed_count++;
      // Not increasing i
      continue;
    }
    // A self reference (loop) will happen
    if (chosen_v == chosen_s){
      cout << "Loop avoided {" << chosen_u << "," << chosen_v << "},{" << chosen_s << "," << chosen_t << "}" << endl;
      notallowed_count++;
      // Not increasing i
      continue;
    }


    vector<int>::iterator it = find(adj_list[chosen_v].begin(), adj_list[chosen_v].end(), chosen_u);
    index_u = distance(adj_list[chosen_v].begin(), it);

    vector<int>::iterator it2 = find(adj_list[chosen_t].begin(), adj_list[chosen_t].end(), chosen_s);
    index_s = distance(adj_list[chosen_t].begin(), it2);

    // cout << "\nExperiment " << i << endl;
    // cout << chosen_u << " - " << chosen_v << endl;
    // cout << chosen_s << " - " << chosen_t << endl;
    // print_adj(chosen_u);
    // print_adj(chosen_v);
    // print_adj(chosen_s);
    // print_adj(chosen_t);
  
    // cout << "###### Perform swap ######" << endl;
    swap(adj_list[chosen_s].at(index_t), adj_list[chosen_u].at(index_v));
    swap(adj_list[chosen_t].at(index_s), adj_list[chosen_v].at(index_u));

    // print_adj(chosen_u);
    // print_adj(chosen_v);
    // print_adj(chosen_s);
    // print_adj(chosen_t);

    i++;
  }
  cout << "Number of not allowed but counted: " << notvalid_count << endl;
  cout << "Number of not valid and not counted: " << notallowed_count << endl;
}

Graph::Graph(const Graph &g) {
  cout << "I am the copy constructor" << endl;
}

Graph::~Graph() {
  cout << "I am being destroyed: " << this << endl;
}

const int Graph::get_num_vertices(){ return adj_list.size(); }
const int Graph::get_num_edges() {
  int sum_edges = 0;
  const int noV = adj_list.size();
  for (int i = 0; i < noV; i++){    
    sum_edges += adj_list[i].size();
    // cout << i;
    // for(int j = 0; j < adj_list[i].size(); j ++) 
    //   cout << "\t" << adj_list[i].at(j);
    // cout << endl;
  }
  return sum_edges/2;
}

const double Graph::closeness_centrality_single(int source){
  // a queue to maintain queue of vertices whose 
  // adjacency list is to be scanned as per normal 
  // DFS algorithm 
  if (adj_list[source].size() == 0) {
    return 0;
  }
  list<int> queue;
  //int pred[nNodes];
  int dist[nNodes];

  // boolean array visited[] which stores the 
  // information whether ith vertex is reached 
  // at least once in the Breadth first search  
  bool visited[nNodes];

  // initially all vertices are unvisited 
  // so v[i] for all i is false 
  // and as no path is yet constructed 
  // dist[i] for all i set to infinity  
  for (int i = 0; i < nNodes; i++) { 
    visited[i] = false; 
    dist[i] = INT_MAX; 
    //pred[i] = -1; 
  }

  // now source is first to be visited and 
  // distance from source to itself should be 0 
  visited[source] = true; 
  dist[source] = 0; 
  queue.push_back(source); 

  // standard BFS algorithm 
  while (!queue.empty()) { 
    int u = queue.front(); 
    queue.pop_front(); 
    int current_length = adj_list[u].size(); 
    for (int i = 0; i < current_length; i++) { 
      if (visited[adj_list[u].at(i)] == false) { 
          visited[adj_list[u].at(i)] = true; 
          dist[adj_list[u].at(i)] = dist[u] + 1; 
          //pred[adj_list[u].at(i)] = u; 
          queue.push_back(adj_list[u].at(i)); 

          // We don't stop BFS when we find 
          // destination. 
          // Let it run to fill dist[]
          // if (adj_list[u].at(i) == dest) 
          //     return dist[dest]; 
      } 
    } 
  } 

  // For all distances
  double accumulator = 0;
  for (int i = 0; i < nNodes; i++){
    if (dist[i] > 0 && dist[i] < INT_MAX){
      //cout << " Distance i " << 1/(dist[i]*1.0) << endl;
      accumulator += 1/(dist[i]*1.0);
    }    
  }
  //cout << "Global thing: " << accumulator << endl;
  return accumulator/(nNodes*1.0 - 1.0);
}

void Graph::global_closeness_centrality(){
  cout << "Calculating Closeness Centrality..." << endl;
  clock_t begin = clock();
  int i;
  double sum_c_i = 0;
  for(i = 0; i < nNodes; i++){
    sum_c_i += closeness_centrality_single(i);
  }
  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  cout << "Global Calculation finished in " << elapsed_secs << " seconds." << endl;
  double result = sum_c_i/(nNodes*1.0);
  cout << "Global Closeness Centrality of is " << result << endl;
}

void Graph::GenerateGraph(int NoE, int NoV){  
  int i, j, edge[NoE][2];  
  for(i = 0; i < NoV; i++){
    adj_list.push_back(vector<int>());
  }
  

  i = 0;

  while(i < NoE)
	{
		edge[i][0] = rand()%NoV;
		edge[i][1] = rand()%NoV;

    // No self edges
		if(edge[i][0] == edge[i][1]){      
			continue;
    }
    else
		{
			for(j = 0; j < i; j++)
			{
        // No multi edges, no cycles
				if((edge[i][0] == edge[j][0] && edge[i][1] == edge[j][1]) || (edge[i][0] == edge[j][1] && edge[i][1] == edge[j][0])){
          i--;
        }					
			}
		}    
		i++;
	}
  for(i = 0; i < NoE; i++){   
    adj_list[edge[i][0]].push_back(edge[i][1]);
    adj_list[edge[i][1]].push_back(edge[i][0]);
  }
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

int index_word(const string &word, unordered_map <string, int> &word_dict) {
  int id = word_dict[word];
  if (!id) {
    id = word_dict.size();
    word_dict[word] = id;
  }
  return id;
}

// This assumes that the first line in the file indicates the number of vertices
// and edges in the graph described by the file. Those numbers are used as a
// reference and not actually included in the graph.
void Graph::read_graph_from_file(string path) {
  int lines_read = 1, exp_v, exp_e, v1, v2;
  string line, w;
  vector<size_t> token_pos;
  ifstream f (path); // Create a file stream for the specified path
  unordered_map <string, int> word_dict;  // Allows for very fast check of existence
  if (!f.is_open()) {
    cerr << "Error: Unable to open file '" << path << "'" << endl;
    return;
  }
  cerr << "Reading graph from file '" << path << "'..." << endl << endl;
  getline(f, line);
  if ( tokenize_line(line, token_pos) != 2 ) {
    cerr << "Error: Malformed first row" << endl;
    return;
  }
  cerr << "Reading header..." << endl;
  // Parse the numbers to integers and store them to check later
  exp_v = stoi( line.substr(token_pos[0], token_pos[1]) );
  exp_e = stoi( line.substr(token_pos[2], token_pos[3]) );

  for(int i = 0; i < exp_v; i++){
    adj_list.push_back(vector<int>());
  }

  cerr << "-----------------------------------------------" << endl;
  cerr << "Expected number of vertices: " << exp_v << endl;
  cerr << "Expected number of edges: " << exp_e << endl;
  cerr << "-----------------------------------------------" << endl << endl;
  cerr << "Reading edges..." << endl;
  cerr << "-----------------------------------------------" << endl;

  while(getline(f, line)) {
    ++lines_read;
    if ( tokenize_line(line, token_pos) != 2 ) {
      cerr << " - Malformed edge at file row " << lines_read << endl;
    } else if (!line.compare(token_pos[0], token_pos[1], line,
                token_pos[2], token_pos[3])) {
      cerr << " - Loop found at file row " << lines_read << endl;
    } else if (!line.compare(token_pos[0], token_pos[1], " ") ||
                !line.compare(token_pos[2], token_pos[3], " ")) {
      cerr << " - Malformed edge at file row " << lines_read << endl;
    } else {
      v1 = index_word(line.substr(token_pos[0], token_pos[1]), word_dict);
      v2 = index_word(line.substr(token_pos[2], token_pos[3]), word_dict);
      adj_list[v1].push_back(v2);
      adj_list[v2].push_back(v1);
    }
  }

}

