#include "graph.h"
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <bits/stdc++.h> 

using namespace std;

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

// int Graph::geodesic_distance(int ix_s, int ix_d){
//   return BFS(ix_s, ix_d);
// }

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

