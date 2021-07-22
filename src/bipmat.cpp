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
    std::string line;
    int val;
    
    std::ifstream stream(input_path);
    if (!stream.is_open()) {
        std::cout << "Could not open input file." << std::endl;
        exit(1);
    }
    
    // Read problem size.
    getline(stream, line);
//    assert line.length() > 0;
    std::istringstream(line) >> this->n;
    
    // Initialize graph once we have the problem size.
    graph = new BipartiteGraph(n);

    // Read all edges.
    while (getline(stream, line))
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
    delete graph;
    
//    for (auto edge : M) {
//        delete edge;
//    }
    
}

    
void BipartiteMatcher::match()
{
//    graph->print_graph();
    std::cout << "Num edges = " << graph->edges.size() << "\n";
    
    std::vector<Edge*> path;
    std::unordered_set<int> v_visited;
    std::unordered_set<int> w_visited;
    
    SearchTree *st = new SearchTree();
    st->set_root(0);
    
    while (M.size() != n) {  // if matching is not perferct
        
        // TODO: Debug
        for (auto e : M) {
            if (!graph->is_tight(e)) {
                exit(1);
            }
        }
        
        // Clear everything for new search.
        
        // DEBUG
//        std::cout << "pv: ";
//        for (int p : graph->v_prices) { std::cout << p << " "; }
//        std::cout << "\n";
//
//        std::cout << "pw: ";
//        for (int p : graph->w_prices) { std::cout << p << " "; }
//        std::cout << "\n";
//
//        std::cout << "M: ";
//        for (Edge* e : M) {
//            std::cout << "(" << e->v << "," << e->w << "," << e->cost << "), ";
//        }
//        std::cout << "\n";
        
//        graph->print_graph();
        
        st->clear();
        
        path.clear();
        v_visited.clear();
        w_visited.clear();
                
        graph->search_good_path(M, st, path, v_visited, w_visited);
        
        if (path.size() != 0) {  // found a good path
            augment(path);
        } else {
            graph->update_prices(v_visited, w_visited);
        }
    }

    int sum = 0;
    for (Edge* e : M) {
//        std::cout << e->v << " " << e->w << " " << e->cost << "\n";
        sum += e->cost;
    }
    
    min_cost = sum;
//    std::cout << "min cost = " << sum << "\n";
    
    delete st;
}


void BipartiteMatcher::augment(std::vector<Edge*> &path)
{
    for (int i = 0; i < path.size(); i++) {
        if (M.find(path[i]) == M.end()) {
            M.insert(path[i]);
        } else {
            M.erase(path[i]);
        }
    }
    
    for (int i = 0; i < n; i++) {
        graph->v_matched[i] = 0;
        graph->w_matched[i] = 0;
    }
    
    for (Edge* edge : M) {
        graph->v_matched[edge->v] |= 1;
        graph->w_matched[edge->w] |= 1;
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
