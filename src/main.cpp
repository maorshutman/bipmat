#include <iostream>

#include "bipmat.h"
#include "test.h"


int main(int argc, const char * argv[]) {
    
    int run_test = 1;
    
    if (!run_test) {
        
        std::string input_path = "/Users/maorshutman/repos/bipmat/tests/inputs/edges_3x3_input_2.txt";
        
        wbm::BipartiteMatcher* matcher = new wbm::BipartiteMatcher(input_path, "edges");
        matcher->match();
        delete matcher;
        
    }
    else {
        test();
    }
    
    
    return 0;
}
