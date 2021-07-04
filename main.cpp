//
//  main.cpp
//  bipmat
//
//  Created by Maor Shutman on 04/07/2021.
//  Copyright © 2021 Maor Shutman. All rights reserved.
//

#include <iostream>

#include "bipmat.h"


int main(int argc, const char * argv[]) {
    
    std::string input_costs_path = "/Users/maorshutman/repos/bipmat/inputs/5x5_input.txt";

    bm::BipartiteMatcher mathcer = bm::BipartiteMatcher(input_costs_path);
    mathcer.print_costs();
    std::cout << "Problem size: " << mathcer.get_n() << " x " << mathcer.get_m() << "\n";
    
    mathcer.match_min();
    
    return 0;
}
