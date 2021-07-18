#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include "bipmat.h"


namespace wbm {

    
BipartiteMatcher::BipartiteMatcher(std::string input_path, std::string format)
{
    if (format == "edges") {
        read_edges(input_path);
    } else if (format == "matrix") {
        read_cost_matrix(input_path);
    } else {
        std::cout << "Invalid input format name. \n";
        exit(1);
    }
}
    
void BipartiteMatcher::read_cost_matrix(std::string input_path)
{
    std::ifstream input(input_path);
    std::string line;
    int val;
    
    // Read problem size.
    getline(input, line);
    std::istringstream(line) >> this->n;
    graph = new BipartiteGraph(this->n);
    
    while (getline(input, line)) {
        std::vector<int> row;
        std::istringstream ss(line);
    
        while (ss >> val) {
            row.push_back(val);
        }
        assert(int(row.size()) == row.size());
        costs.push_back(row);
    }
    
    // Add edges.
    for (int v = 0; v < n; v++) {
        for (int w = 0; w < n; w++) {
            graph->add_edge(v, w, costs[v][w]);
        }
    }
}
    

void BipartiteMatcher::read_edges(std::string input_path)
{
    std::ifstream input(input_path);
    std::string line;
    int val;
    
    // Read problem size.
    getline(input, line);
    std::istringstream(line) >> this->n;
    
    // Initialize graph once we have the problem size.
    graph = new BipartiteGraph(n);

    // Read all edges.
    while (getline(input, line))
    {
        std::vector<int> row;
        std::istringstream ss(line);
        
        while (ss >> val) {
            row.push_back(val);
        }
        
        graph->add_edge(row[0], row[1], row[2]);
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

    int sum = 0;
    for (Edge* e : M) {
        std::cout << e->v << " " << e->w << " " << e->cost << "\n";
        sum += e->cost;
    }
    
    min_cost = sum;
    std::cout << "min cost = " << sum << "\n";
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
