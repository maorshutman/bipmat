#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include "bipmat.h"


namespace wbm {

BipartiteMatcher::BipartiteMatcher(std::string input_path)
{
    std::ifstream input(input_path);
    std::string line;
    int val;
    n = 0;
    
    while (getline(input, line))
    {
        std::vector<int> row;
        std::istringstream ss(line);
    
        while (ss >> val) {
            row.push_back(val);
        }
        
        if (n == 0) {
            n = int(row.size());
        } else {
            assert(n == row.size());
        }
        
        costs.push_back(row);
    }
    
}

    
BipartiteMatcher::BipartiteMatcher(int n) {
    this->n = n;
    graph = new BipartiteGraph(n);
}


BipartiteMatcher::~BipartiteMatcher()
{
}

    
void BipartiteMatcher::match()
{
    graph->print_graph();
    std::cout << "Num edges = " << graph->edges.size() << "\n";
    
    std::vector<Edge*> path;
    std::vector<Edge*> set;
    
//    SearchTree* st = nullptr;
    
    while (M.size() != n) {  // if matching is not perferct
        
        path.clear();
        set.clear();
        
        graph->search_good_path(path, set);
        
        if (path.size() != 0) {  // found a good path
            augment(path);
        } else {
            
            graph->update_prices(set);
        }
        
    }

}


void BipartiteMatcher::augment(std::vector<Edge*> &path)
{
    for (int i = 0; i < path.size(); i++) {
        if (!(M.find(path[i]) != M.end())) {
            M.insert(path[i]);
            graph->v_matched[path[i]->v] = 1;
            graph->w_matched[path[i]->w] = 1;
        } else {
            M.erase(path[i]);
            graph->v_matched[path[i]->v] = 0;
            graph->w_matched[path[i]->w] = 0;
        }
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
    
    
void BipartiteMatcher::print_matching()
{
}
    
}
