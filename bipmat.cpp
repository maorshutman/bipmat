//
//  bipmat.cpp
//  bipmat
//
//  Created by Maor Shutman on 04/07/2021.
//  Copyright © 2021 Maor Shutman. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include "bipmat.h"


namespace bm {

BipartiteMatcher::BipartiteMatcher(std::string input_path)
{
    std::ifstream input(input_path);
    std::string line;
    float val;
    
    while (getline(input, line))
    {
        std::vector<float> row;
        std::istringstream ss(line);
    
        while (ss >> val) {
            row.push_back(val);
        }
        
        if (n == 0) {
            n = row.size();
        } else {
            assert(n == row.size());
        }
        
        costs.push_back(row);
    }
    
    m = costs.size();
}


BipartiteMatcher::~BipartiteMatcher()
{
}

    
void BipartiteMatcher::match_max()
{
}

    
void BipartiteMatcher::match_min()
{
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
    
}
