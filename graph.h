#ifndef graph_h
#define graph_h


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

    TreeNode(int idx) {
        this->idx = idx;
        parent = nullptr;
        parent_edge = nullptr;
    }
    
    void add_child (TreeNode *node) {
        node->parent = this;
        children.push_back(node);
    }
};


struct SearchTree {
    TreeNode *root;
    std::vector<TreeNode*> leafs;
    
    SearchTree(int k) {
        root = new TreeNode(k);
        leafs.push_back(root);
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

    ~BipartiteGraph() {}

    void add_edge(int v, int w, int cost) {
        Edge* edge = new Edge(v, w, cost);
        edges.insert(edge);
        V[v].push_back(edge);
        W[w].push_back(edge);
    }

    void print_graph() {
        std::cout << "V:" << "\n";
        for (int i = 0; i < V.size(); i++) {
            std::cout << i << "-> ";
            for (int j = 0; j < V[i].size(); j++) {
                std::cout << "(" << V[i][j]->v << "," << V[i][j]->w << "," << V[i][j]->cost << "), ";
            }
            std::cout << "\n";
        }

        std::cout << "W:" << "\n";
        for (int i = 0; i < W.size(); i++) {
            std::cout << i << "-> ";
            for (int j = 0; j < W[i].size(); j++) {
                std::cout << "(" << W[i][j]->v << "," << W[i][j]->w << "," << W[i][j]->cost << "), ";
            }
            std::cout << "\n";
        }
    }

    void search_good_path(std::vector<Edge*> &path, std::vector<Edge*> &set) {
        
        std::unordered_set<int> v_visited;
        std::unordered_set<int> w_visited;
        size_t prev_num_visited_vs = 0;
        size_t prev_num_visited_ws = 0;
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
        
        // TODO: make st a member?
        SearchTree st(r);
        TreeNode* path_head = nullptr;
        
        int level_even = 0;
        
        while (!stuck) {
            if (level_even) { // next level is odd
                path_head = bfs_step_even_level(st, w_visited);
                if (path_head) { break; }
            } else { // next level is even
                bfs_step_odd_level(st, v_visited);
            }
            
            // Check if search is stuck.
            if ((prev_num_visited_vs == v_visited.size()) &&
                (prev_num_visited_ws == w_visited.size())) {
                stuck = 1;
            }
            prev_num_visited_vs = v_visited.size();
            prev_num_visited_ws = w_visited.size();
            
            level_even ^= 1;
        }

        if (path_head) { // found a good path
            // `path_head` starts in W by definition.
            while (path_head && path_head->parent_edge) {
                path.push_back(path_head->parent_edge);
                path_head = path_head->parent;
            }
        } else { // find a good set
            find_good_set(st, set);
        }
        
    }
        
    TreeNode* bfs_step_even_level(SearchTree &st, std::unordered_set<int> &w_visited)
    {
        int v, w;
        int found = 0;
        std::vector<TreeNode*> new_leafs;
        TreeNode* path_head = nullptr;
        
        // Here all leafs are in V.
        for (TreeNode* leaf : st.leafs) {
            v = leaf->idx;
            
            for (Edge* edge : V[v]) {
                w = edge->w;

                if (is_tight(edge) && (w_visited.find(w) == w_visited.end())) {
                    // Add to a node in W to search tree.
                    TreeNode *new_node = new TreeNode(w);
                    new_node->parent_edge = edge;
                    leaf->add_child(new_node);
                    new_leafs.push_back(new_node);
                    
                    w_visited.insert(w);
                    
                    if (!w_matched[w]) {
                        // Found a good path.
                        found = 1;
                        path_head = new_node;
                        break;
                    }
                }
                
            }
        }

        if (!found) {
            // Update search tree leafs for next step.
            st.leafs.clear();
            for (TreeNode* node : new_leafs) {
                st.leafs.push_back(node);
            }
            return nullptr;
        } else {
            return path_head;
        }
    }
    
    
    TreeNode* bfs_step_odd_level(SearchTree &st, std::unordered_set<int> &v_visited)
    {
        int v, w;
        std::vector<TreeNode*> new_leafs;

        // Here all leafs are in W.
        for (TreeNode* leaf : st.leafs) {
            w = leaf->idx;
            
            for (Edge* edge : W[w]) {
                v = edge->v;

                // if matched, tight by definition
                if ((v_visited.find(v) == v_visited.end()) && v_matched[v]) {
                    // Add to a node in V to search tree.
                    TreeNode *new_node = new TreeNode(v);
                    new_node->parent_edge = edge;
                    leaf->add_child(new_node);
                    new_leafs.push_back(new_node);
                    
                    v_visited.insert(v);
                }
            }
        }
    
        // Update search tree leafs for next step.
        st.leafs.clear();
        for (TreeNode* node : new_leafs) {
            st.leafs.push_back(node);
        }
        
        return nullptr;
    }
    
    
    void find_good_set(SearchTree &st, std::vector<Edge*> &set)
    {
    }
    
    
    int is_tight(Edge* edge) {
        return (edge->cost - v_prices[edge->v] - w_prices[edge->w] == 0);
    }
    
    
    void update_prices(std::vector<Edge*> &set)
    {
    }
    
};


#endif /* graph_h */
