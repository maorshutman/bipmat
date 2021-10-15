
#include "graph.h"

  
BipartiteGraph::BipartiteGraph(int n) {
  this->n = n;
  
  for (int i = 0; i < n; ++i) {
    V.push_back(std::vector<Edge*>());
    W.push_back(std::vector<Edge*>());
    v_prices.push_back(0);
    w_prices.push_back(0);
    v_matched.push_back(0);
    w_matched.push_back(0);
    
    slack_w.push_back(0);
    slack_w_flag.push_back(0);
    
    match_mat.push_back(std::unordered_set<int>());
  }
  
}

BipartiteGraph::~BipartiteGraph() {
  for (auto edge : edges) {
    delete edge;
  }
}

void BipartiteGraph::add_edge(int v, int w, int cost) {
  Edge* edge = new Edge(v, w, cost);
  edges.insert(edge);
  V[v].push_back(edge);
  W[w].push_back(edge);
}

void BipartiteGraph::search_augmenting_path(std::unordered_set<Edge*> M,
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

TreeNode* BipartiteGraph::bfs_step_even_level(SearchTree *st)
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

TreeNode* BipartiteGraph::bfs_step_odd_level(SearchTree *st, std::unordered_set<Edge*> M)
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
        
        update_slack_upon_new_v(v);
      }
    }
  }
  
  return nullptr;
}

int BipartiteGraph::update_prices()
{
  int delta = compute_update_delta();
  
  for (int v : v_visited) {
    v_prices[v] += delta;
  }
  for (int w : w_visited) {
    w_prices[w] -= delta;
  }
  
  update_slack_upon_price_update(delta);
  
  return delta;
}

inline int BipartiteGraph::compute_update_delta() {
  int min = INT_MAX;
  for (int w = 0; w < n; w++) {
    if ((w_visited.find(w) == w_visited.end()) && slack_w_flag[w]) {
      if (slack_w[w] < min) {
        min = slack_w[w];
      }
    }
  }
  return min;
}

int BipartiteGraph::compute_update_delta_dbg()
  {
    // Consider edges s.t. v in S, w not in N(S). Maximal delta that zeros one
    // of these edges.
    
    int delta = INT_MAX;
    int rc;
    for (int v : v_visited) {  // v is in S
      for (Edge* edge : V[v]) {
        // If w is not in N(S)
        if (w_visited.find(edge->w) == w_visited.end()) {
          rc = reduced_cost(edge);
          if (rc < delta) {
            delta = rc;
          }
        }
      }
    }
    
    return delta;
  }

void BipartiteGraph::init_slack(int root) {
  for (int w = 0; w < n; w++) {
    slack_w_flag[w] = 0;
    slack_w[w] = INT_MAX;
  }
  
  for (Edge* edge : V[root]) {
    slack_w[edge->w] = reduced_cost(edge);
    slack_w_flag[edge->w] = 1;
  }
}

inline void BipartiteGraph::update_slack_upon_new_v(int v) {
  for (Edge* edge : V[v]) {
    if (w_visited.find(edge->w) == w_visited.end()) {
      slack_w_flag[edge->w] = 1;
      int rc = reduced_cost(edge);
      if (rc < slack_w[edge->w]) {
        slack_w[edge->w] = rc;
      }
    }
  }
}

inline void BipartiteGraph::update_slack_upon_price_update(int delta) {
  for (int w = 0; w < n; w++) { // n
    if ((w_visited.find(w) == w_visited.end()) && slack_w_flag[w]) {
      slack_w[w] -= delta;
    }
  }
}

inline int BipartiteGraph::reduced_cost(Edge* edge) {
  return edge->cost - v_prices[edge->v] - w_prices[edge->w];
}

inline int BipartiteGraph::is_tight(Edge* edge) {
  return reduced_cost(edge) == 0;
}

inline int BipartiteGraph::is_matched(Edge* edge, std::unordered_set<Edge*> &M) {
  return (match_mat[edge->v].find(edge->w) != match_mat[edge->v].end());
}

int BipartiteGraph::init_price(int v) {
  int min = INT_MAX;
  for (Edge* edge : V[v]) {
    if (edge->cost < min) {
      min = edge->cost;
    }
  }
  return min;
}
