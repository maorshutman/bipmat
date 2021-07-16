#ifndef bipmat_h
#define bipmat_h

#include <vector>
#include <unordered_set>
#include "graph.h"


namespace wbm { class BipartiteMatcher {
    private:
        int n;
        std::vector<std::vector<int>> costs;
        BipartiteGraph* graph;
        std::unordered_set<Edge*> M;
    
        void augment(std::vector<Edge*> &path);
        
    public:
        BipartiteMatcher(std::string input_path, std::string format);
        BipartiteMatcher(int n);
        ~BipartiteMatcher();
        
        void read_cost_matrix(std::string input_path);
        void read_edges(std::string input_path);
        void add_edge(int v, int w, int cost);
        void match();
        
        void print_costs();
        void print_matching();
        int get_n() { return n; }
    
};

}

#endif /* bipmat_h */
