#ifndef bipmat_h
#define bipmat_h

#include <vector>
#include <unordered_set>
#include "graph.h"


namespace wbm { class BipartiteMatcher {
    private:
        std::vector<std::vector<int>> costs;
        int n;
    
        BipartiteGraph* graph;
        std::unordered_set<Edge*> M;
    
    public:
        BipartiteMatcher(std::string input_path);
        BipartiteMatcher(int n);
        ~BipartiteMatcher();
        
        void add_edge(int v, int w, int cost);
        void match();
        void print_costs();
        void print_matching();
        int get_n() { return n; }
    
};

}

#endif /* bipmat_h */
