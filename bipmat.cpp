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
    this->graph = new BipartiteGraph(n);
}


BipartiteMatcher::~BipartiteMatcher()
{
}

    
void BipartiteMatcher::match()
{
    graph->print_graph();
    
    // TODO
}


void BipartiteMatcher::add_edge(int v, int w, int cost) {
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
    
    
void BipartiteMatcher::print_matching() {

}
    
}
