#include <iostream>

#include "bipmat.h"
#include "test.h"


int main(int argc, const char * argv[]) {
    
    int run_test = 1;
    
    if (!run_test) {
        
        std::string input_path = "/Users/maorshutman/repos/bipmat/tests/inputs/edges_7x7_input_69.txt";
        
        wbm::BipartiteMatcher* matcher = new wbm::BipartiteMatcher(input_path, "edges");
        matcher->match();
        std::cout << "min cost " << matcher->get_min_cost() << std::endl;
        delete matcher;
        
    }
    else {
        test();
    }
    
    
    return 0;
}
