#ifndef graph_h
#define graph_h

#include <stdio.h>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>
#include "vector_set_t.h"

#define SIDE_V 0
#define SIDE_W 1


struct Edge {
  int v;
  int w;
  int cost;
  
  Edge(int v, int w, int cost) :
    v(v), w(w), cost(cost) {}
};


struct TreeNode {
  int idx;
  TreeNode *parent;
  Edge *parent_edge;
  std::vector<TreeNode*> children;
  unsigned char side;
  
  TreeNode(int idx) {
    this->idx = idx;
    parent = nullptr;
    parent_edge = nullptr;
  }
  
  ~TreeNode() {}
  
  void add_child (TreeNode *node) {
    node->parent = this;
    children.push_back(node);
  }
};


struct SearchTree {
  TreeNode *root;
  std::unordered_set<TreeNode*> leafs;
  
  SearchTree() {}
  
  ~SearchTree() {
    delete_nodes(root);
  }
  
  void set_root(int k) {
    root = new TreeNode(k);
    root->side = SIDE_V;
    leafs.insert(root);
  }
  
  void delete_nodes(TreeNode *node) {
    if (!node) {
      return;
    }
    for (TreeNode* nd : node->children) {
      delete_nodes(nd);
    }
    delete node;
  }
  
  void clear() {
    delete_nodes(root);
    root = nullptr;
    leafs.clear();
  }
  
  void add_leaf(TreeNode* parent, TreeNode* leaf) {
    if (leafs.find(parent) != leafs.end()) {
      leafs.erase(parent);
    }
    leafs.insert(leaf);
  }
};


struct BipartiteGraph {
  int n;
  std::vector<std::vector<Edge*>> V;
  std::vector<std::vector<Edge*>> W;
  std::unordered_set<Edge*> edges;
  std::vector<int> v_matched;
  std::vector<int> w_matched;
  std::vector<int> v_prices;
  std::vector<int> w_prices;
  std::unordered_set<int> v_visited;
  std::unordered_set<int> w_visited;

//  vector_set_t v_visited_vec;
//  vector_set_t w_visited_vec;
  std::vector<std::unordered_set<int>> match_mat;
  
  BipartiteGraph(int n) {
    for (int i = 0; i < n; ++i) {
      V.push_back(std::vector<Edge*>());
      W.push_back(std::vector<Edge*>());
      v_prices.push_back(0);
      w_prices.push_back(0);
      v_matched.push_back(0);
      w_matched.push_back(0);
      match_mat.push_back(std::unordered_set<int>());
    }
            
//    v_visited_vec = vector_set_t(n);
//    w_visited_vec = vector_set_t(n);
    
    this->n = n;
  }
  
  ~BipartiteGraph() {
    for (auto edge : edges) {
      delete edge;
    }
  }
  
  void add_edge(int v, int w, int cost) {
    Edge* edge = new Edge(v, w, cost);
    edges.insert(edge);
    V[v].push_back(edge);
    W[w].push_back(edge);
  }
  
  void search_augmenting_path(std::set<Edge*> M,
                              SearchTree *st,
                              std::vector<Edge*> &path)
  {
    
    int stuck = 0;
    TreeNode* path_head = nullptr;
    size_t prev_num_visited_vs = v_visited.size();
    size_t prev_num_visited_ws = w_visited.size();
    
    int level_even = 1;
    
    while (!stuck) {
      if (level_even) { // next level is odd
        path_head = bfs_step_even_level(st);
        if (path_head) {
          break;
        }
        
        if (prev_num_visited_ws == w_visited.size()) {
          stuck = 1;
        }
        prev_num_visited_ws = w_visited.size();
        
      } else { // next level is even
        bfs_step_odd_level(st, M);
        
        if (prev_num_visited_vs == v_visited.size()) {
          stuck = 1;
        }
        prev_num_visited_vs = v_visited.size();
      }
      
      level_even ^= 1;
    }
    
    if (path_head) { // found an augmenting path
      // `path_head` starts in W by definition.
      while (path_head && path_head->parent_edge) {
        path.push_back(path_head->parent_edge);
        path_head = path_head->parent;
      }
    }
  }
  
  TreeNode* bfs_step_even_level(SearchTree *st)
  {
    int v, w;
    int found = 0;
    TreeNode* path_head = nullptr;
    
    auto leafs_cp = st->leafs;
    
    for (TreeNode* leaf : leafs_cp) {
      
      if (leaf->side != SIDE_V) {
        continue;
      }
      
      v = leaf->idx;
      
      for (Edge* edge : V[v]) {
        w = edge->w;
        
        if (is_tight(edge) && (w_visited.find(w) == w_visited.end())) {
          // Add to a node in W to search tree.
          TreeNode *new_node = new TreeNode(w);
          new_node->parent_edge = edge;
          new_node->side = SIDE_W;
          leaf->add_child(new_node);
          
          st->add_leaf(leaf, new_node);
          
          w_visited.insert(w);
//          w_visited_vec.insert(w);
          
          // Found a good path.
          if (!w_matched[w]) {
            found = 1;
            path_head = new_node;
            break;
          }
        }
      }
    }
    
    return path_head;
  }
  
  TreeNode* bfs_step_odd_level(SearchTree *st, std::set<Edge*> M)
  {
    int v, w;
    
    auto leafs_cp = st->leafs;
    
    for (TreeNode* leaf : leafs_cp) {
      w = leaf->idx;
      
      if (leaf->side != SIDE_W) {
        continue;
      }
      
      for (Edge* edge : W[w]) {
        v = edge->v;
        
        // If matched, tight by definition
        if ((v_visited.find(v) == v_visited.end()) && is_matched(edge, M)) {
          // Add to a node in V to search tree.
          TreeNode *new_node = new TreeNode(v);
          new_node->parent_edge = edge;
          new_node->side = SIDE_V;
          leaf->add_child(new_node);
          st->add_leaf(leaf, new_node);
          
          v_visited.insert(v);
//          v_visited_vec.insert(v);
          
        }
      }
    }
    
    return nullptr;
  }
  
  int update_prices()
  {
    // good_set -> in V, even levels
    // neighb_good_set -> in W, odd levels

    int delta = compute_update_delta();
    
    for (int v : v_visited) {
      v_prices[v] += delta;
    }
    for (int w : w_visited) {
      w_prices[w] -= delta;
    }
    
    return delta;
  }
  
  int compute_update_delta()
  {
    // Consider edges s.t. v in S, w not in N(S). Maximal delta that zeros one
    // of these edges.
    
    int delta = INT_MAX;
    int rc;
    for (int v : v_visited) {  // v is in S
      for (Edge* edge : V[v]) {
        // If w is not in N(S)
        if (w_visited.find(edge->w) == w_visited.end()) {
//        if (!w_visited_vec.find(edge->w)) {
          rc = reduced_cost(edge);
          if (rc < delta) {
            delta = rc;
          }
        }
      }
    }
    
    return delta;
  }
  
  inline int reduced_cost(Edge* edge) {
    return edge->cost - v_prices[edge->v] - w_prices[edge->w];
  }
  
  inline int is_tight(Edge* edge) {
    return reduced_cost(edge) == 0;
  }
  
  inline int is_matched(Edge* edge, std::set<Edge*> &M) {
//    return match_mat[edge->v][edge->w];
//    return (M.find(edge) != M.end());
    return (match_mat[edge->v].find(edge->w) != match_mat[edge->v].end());
//    return edge->in_match;
  }
  
  void print_graph() {
    std::cout << "V:" << "\n";
    for (int i = 0; i < V.size(); i++) {
      std::cout << i << "-> ";
      for (int j = 0; j < V[i].size(); j++) {
        std::cout << "(" << V[i][j]->v << "," << V[i][j]->w << "," << V[i][j]->cost << "," <<  reduced_cost(V[i][j]) << "), ";
      }
      std::cout << "\n";
    }
    
    std::cout << "W:" << "\n";
    for (int i = 0; i < W.size(); i++) {
      std::cout << i << "-> ";
      for (int j = 0; j < W[i].size(); j++) {
        std::cout << "(" << W[i][j]->v << "," << W[i][j]->w << "," << W[i][j]->cost << "," <<  reduced_cost(W[i][j]) << "), ";
      }
      std::cout << "\n";
    }
  }
  
};


#endif /* graph_h */
