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
    std::unordered_set<int> v_visited;
    std::unordered_set<int> w_visited;
    SearchTree *st = new SearchTree();
    
    while (M.size() != n) {  // if matching is not perferct
        // Clear everything for new search.
        
        // DEBUG
        std::cout << "pv: ";
        for (int p : graph->v_prices) { std::cout << p << " "; }
        std::cout << "\n";
        
        std::cout << "pw: ";
        for (int p : graph->w_prices) { std::cout << p << " "; }
        std::cout << "\n";
        
        std::cout << "M: ";
        for (Edge* e : M) {
            std::cout << "(" << e->v << "," << e->w << "," << e->cost << "), ";
        }
        std::cout << "\n";
        
        graph->print_graph();
        
//        st->delete_nodes(st->root);
//        st->leafs.clear();
        st->clear();
        
        path.clear();
        v_visited.clear();
        w_visited.clear();
                
        graph->search_good_path(st, path, v_visited, w_visited);
        
        if (path.size() != 0) {  // found a good path
            augment(path);
        } else {
            graph->update_prices(v_visited, w_visited);
        }
    }

    
    for (Edge* e : M) {
        std::cout << e->v << " " << e->w << " " << e->cost << "\n";
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
