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
  std::vector<std::unordered_set<int>> match_mat;
  
  std::vector<int> slack_w;
  std::vector<int> slack_w_flag;
  

  BipartiteGraph(int n);
  ~BipartiteGraph();
  
  void add_edge(int v, int w, int cost);
  
  void search_augmenting_path(std::unordered_set<Edge*> M, SearchTree *st, std::vector<Edge*> &path);
  TreeNode* bfs_step_even_level(SearchTree *st);
  TreeNode* bfs_step_odd_level(SearchTree *st, std::unordered_set<Edge*> M);
  
  int init_price(int v);
  int update_prices();
  
  void init_slack(int root);
  inline int compute_update_delta();
  int compute_update_delta_dbg();
  
  inline void update_slack_upon_new_v(int v);
  inline void update_slack_upon_price_update(int delta);
  
  inline int reduced_cost(Edge* edge);
  inline int is_tight(Edge* edge);
  inline int is_matched(Edge* edge, std::unordered_set<Edge*> &M);
  
  void print_graph();

};

#endif /* graph_h */
