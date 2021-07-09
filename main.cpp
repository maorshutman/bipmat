#include <iostream>

#include "bipmat.h"


int main(int argc, const char * argv[]) {
    
//    std::string input_costs_path = "/Users/maorshutman/repos/bipmat/inputs/5x5_input.txt";
//    bm::BipartiteMatcher mathcer = bm::BipartiteMatcher(input_costs_path);
//    mathcer.print_costs();
//    std::cout << "Problem size: " << mathcer.get_n() << " x " << mathcer.get_m() << "\n";

    int n = 2;
    wbm::BipartiteMatcher mathcer = wbm::BipartiteMatcher(n);
    
    mathcer.add_edge(0, 0, 2);
    mathcer.add_edge(0, 1, 3);
    mathcer.add_edge(1, 0, 5);
    mathcer.add_edge(1, 1, 7);

    mathcer.match();
    
    return 0;
}
