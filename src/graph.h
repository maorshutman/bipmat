#ifndef graph_h
#define graph_h

#include <stdio.h>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>


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
    char side;
    
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
    std::vector<TreeNode*> leafs;
    
    SearchTree() {}
    
    ~SearchTree() {
        delete_nodes(root);
    }
        
    void set_root(int k) {
        root = new TreeNode(k);
        root->side = 'V'; // TODO: Remove.
        leafs.push_back(root);
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
    
    // TODO: Only for debug, replace by inceremetal changes.
    void update_leafs() {
        leafs.clear();
        search_leafs(root);
    }
    
    void search_leafs(TreeNode* node) {
        if (!node) {
            return;
        }
        if (node->children.size() == 0) {
            leafs.push_back(node);
        } else {
            for (TreeNode* nd : node->children) {
                search_leafs(nd);
            }
        }
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

    BipartiteGraph(int n) {
        for (int i = 0; i < n; ++i) {
            V.push_back(std::vector<Edge*>());
            W.push_back(std::vector<Edge*>());
            
            v_prices.push_back(0);
            w_prices.push_back(0);
            
            v_matched.push_back(0);
            w_matched.push_back(0);
        }
        
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

    void search_good_path(std::unordered_set<Edge*> M,
                          SearchTree *st,
                          std::vector<Edge*> &path,
                          std::unordered_set<int> &v_visited,
                          std::unordered_set<int> &w_visited)
    {
                
        int stuck = 0;
        
        // Find a vertex in V to start with.
        int r = -1;
        for (int i = 0; i < n; i++) {
            if (!v_matched[i]) {
                r = i;
                break;
            }
        }
        // No good path, since there are no unmatches v's.
        if (r == -1) { return; }
        std::cout << "chosen r = " << r << "\n";
        
        
        // TODO: If updated the prices, can continue with the same tree, and save work ?
        st->set_root(r);
        TreeNode* path_head = nullptr;
        v_visited.insert(r);
        
        size_t prev_num_visited_vs = v_visited.size();
        size_t prev_num_visited_ws = w_visited.size();
        
        int level_even = 1;
        
        while (!stuck) {
            
            st->update_leafs();
            
            if (level_even) { // next level is odd
                path_head = bfs_step_even_level(st, w_visited);
                if (path_head) { break; }
        
                // Check if stuck.
                if (prev_num_visited_ws == w_visited.size()) { stuck = 1; }
                prev_num_visited_ws = w_visited.size();
                
            } else { // next level is even
                bfs_step_odd_level(st, v_visited, M);
                
                // Check if stuck.
                if (prev_num_visited_vs == v_visited.size()) { stuck = 1; }
                prev_num_visited_vs = v_visited.size();
            }
                        
            level_even ^= 1;
        }

        if (path_head) { // found a good path
            // `path_head` starts in W by definition.
            while (path_head && path_head->parent_edge) {
                path.push_back(path_head->parent_edge);
                path_head = path_head->parent;
            }
        }
    }

    TreeNode* bfs_step_even_level(SearchTree *st,
                                  std::unordered_set<int> &w_visited)
    {
        int v, w;
        int found = 0;
        std::vector<TreeNode*> new_leafs;
        TreeNode* path_head = nullptr;
        
        // Here all leafs are in V.
        st->update_leafs();
        
        for (TreeNode* leaf : st->leafs) {
            v = leaf->idx;
            
            for (Edge* edge : V[v]) {
                w = edge->w;
                
//                std::cout << "tight ?" << is_tight(edge) << "\n";
                
                if (is_tight(edge) && (w_visited.find(w) == w_visited.end())) {
                    // Add to a node in W to search tree.
                    TreeNode *new_node = new TreeNode(w);
                    new_node->parent_edge = edge;
                    new_node->side = 'W';
                    leaf->add_child(new_node);
                    
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
    
    TreeNode* bfs_step_odd_level(SearchTree *st,
                                 std::unordered_set<int> &v_visited,
                                 std::unordered_set<Edge*> M)
    {
        int v, w;

        // Here all leafs are in W.
        st->update_leafs();
        
        for (TreeNode* leaf : st->leafs) {
            w = leaf->idx;
            
            for (Edge* edge : W[w]) {
                v = edge->v;

                // If matched, tight by definition
                if ((v_visited.find(v) == v_visited.end()) && (M.find(edge) != M.end())) {
                    // Add to a node in V to search tree.
                    TreeNode *new_node = new TreeNode(v);
                    new_node->parent_edge = edge;
                    new_node->side = 'V';
                    leaf->add_child(new_node);
                    
                    v_visited.insert(v);
                }
            }
        }

        return nullptr;
    }

    void update_prices(std::unordered_set<int> &good_set,
                       std::unordered_set<int> &neighb_good_set)
    {
        // good_set -> in V, even levels
        // neighb_good_set -> in W, odd levels
        
        std::cout << good_set.size() << " " << neighb_good_set.size() << "\n";

        // v in S, w not in N(S). Maximal delta that zeros one of these edges.
        // TODO
        int delta = 1e9;
        int rc;
        for (int v : good_set) {  // v in S
            for (Edge* edge : V[v]) {
                if (neighb_good_set.find(edge->w) == neighb_good_set.end()) {  // w not in N(S)
                    rc = reduced_cost(edge);
                    if (rc < delta) { delta = rc; }
                }
            }
        }
                
        for (int v : good_set) {
            v_prices[v] += delta;
        }
        for (int w : neighb_good_set) {
            w_prices[w] -= delta;
        }
        
//        // TODO: debug
//        print_graph();
    }

    int compute_update_delta() {
        // v in S, w not in N(S). Maximal delta that zeros one of these edges.
                
        return 0;
    }

    inline int reduced_cost(Edge* edge) {
        return edge->cost - v_prices[edge->v] - w_prices[edge->w];
    }
    
    int is_tight(Edge* edge) {
        return reduced_cost(edge) == 0;
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
