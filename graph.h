#ifndef graph_h
#define graph_h


#include <vector>
#include <unordered_set>


struct Edge {
    int v;
    int w;
    int cost;
        
    Edge(int v, int w, int cost) : v(v), w(w), cost(cost) {}
    
};


struct BipartiteGraph {
    std::vector<std::vector<Edge*>> V;
    std::vector<std::vector<Edge*>> W;

    std::unordered_set<Edge*> edges;
    
    std::vector<int> v_prices;
    std::vector<int> w_prices;

    BipartiteGraph(int n) {
        for (int i = 0; i < n; ++i) {
            V.push_back(std::vector<Edge*>());
            W.push_back(std::vector<Edge*>());
            v_prices.push_back(0);
            w_prices.push_back(0);
        }
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

    void bfs() {
        // TODO
    }
    
};


#endif /* graph_h */
