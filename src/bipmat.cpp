#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include "bipmat.h"


namespace wbm {
  
  BipartiteMatcher::BipartiteMatcher(std::string input_path, std::string format)
  {
    if (format == "edges") {
      read_edges(input_path);
    } else if (format == "matrix") {
      read_cost_matrix(input_path);
    } else {
      std::cout << "Invalid input format name. \n";
      exit(1);
    }
  }
  
  void BipartiteMatcher::read_cost_matrix(std::string input_path)
  {
    std::ifstream input(input_path);
    std::string line;
    int val;
    
      // Read problem size.
    getline(input, line);
    std::istringstream(line) >> this->n;
    graph = new BipartiteGraph(this->n);
    
    while (getline(input, line)) {
      std::vector<int> row;
      std::istringstream ss(line);
      
      while (ss >> val) {
        row.push_back(val);
      }
      assert(int(row.size()) == row.size());
      costs.push_back(row);
    }
    
      // Add edges.
    for (int v = 0; v < n; v++) {
      for (int w = 0; w < n; w++) {
        graph->add_edge(v, w, costs[v][w]);
      }
    }
  }
  
  
  void BipartiteMatcher::read_edges(std::string input_path)
  {
    std::string line;
    int val;
    
    std::ifstream stream(input_path);
    if (!stream.is_open()) {
      std::cout << "Could not open input file." << std::endl;
      exit(1);
    }
    
    // Read problem size.
    getline(stream, line);
    std::istringstream(line) >> this->n;
    
    // Initialize graph once we have the problem size.
    graph = new BipartiteGraph(n);
    
    // Read all edges.
    while (getline(stream, line))
    {
      std::vector<int> row;
      std::istringstream ss(line);
      
      while (ss >> val) {
        row.push_back(val);
      }
      
      graph->add_edge(row[0], row[1], row[2]);
    }
  }
  
  
  BipartiteMatcher::BipartiteMatcher(int n) {
    this->n = n;
    graph = new BipartiteGraph(n);
    min_cost = 0;
  }
  
  
  BipartiteMatcher::~BipartiteMatcher()
  {
    delete graph;
  }
  
  
  void BipartiteMatcher::match()
  {
    std::vector<Edge*> path;
    unsigned char rebuild_tree = 1;
    
    SearchTree *st = new SearchTree();
    st->set_root(0);
    
    while (M.size() != n) {  // if matching is not perferct
      if (rebuild_tree) {
        st->clear();
        path.clear();

        graph->v_visited.clear();
        graph->w_visited.clear();
//        graph->v_visited_vec.clear();
//        graph->w_visited_vec.clear();
        
        // Find a vertex in V to start with.
        int r = -1;
        for (int i = 0; i < n; i++) {
          if (!graph->v_matched[i]) {
            r = i;
            break;
          }
        }
        // No good path, since there are no unmatches v's.
        if (r == -1) {
          break;
        }

        st->set_root(r);
        graph->v_visited.insert(r);
      }
      
      graph->search_augmenting_path(M, st, path);
      
      if (path.size() != 0) {  // found a good path
        augment(path);
        rebuild_tree = 1;
      } else {
        int delta = graph->update_prices();
        
        if (delta != 0) {
          rebuild_tree = 0;
        } else {
          rebuild_tree = 1;  // TODO: Why this happens ?
        }
      }
    }
    
    min_cost = 0;
    for (Edge* e : M) {
      min_cost += e->cost;
    }
    
    delete st;
  }
  
  
  void BipartiteMatcher::augment(std::vector<Edge*> &path)
  {
    for (int i = 0; i < path.size(); i++) {
      if (M.find(path[i]) == M.end()) {
        M.insert(path[i]);
        graph->match_mat[path[i]->v].insert(path[i]->w);
      } else {
        M.erase(path[i]);
        graph->match_mat[path[i]->v].erase(path[i]->w);
      }
    }
    
    for (int i = 0; i < n; i++) {
      graph->v_matched[i] = 0;
      graph->w_matched[i] = 0;
    }
    
    for (Edge* edge : M) {
      graph->v_matched[edge->v] |= 1;
      graph->w_matched[edge->w] |= 1;
    }
  }
  
  
  void BipartiteMatcher::add_edge(int v, int w, int cost)
  {
    graph->add_edge(v, w, cost);
  }
  
  
  void BipartiteMatcher::print_costs()
  {
    for (int i = 0; i < costs.size(); i++) {
      for (int j = 0; j < costs[0].size(); j++) {
        std::cout << costs[i][j] << " ";
      }
      std::cout << "\n";
    }
  }
  
}
