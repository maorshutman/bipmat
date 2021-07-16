#include <iostream>

#include "bipmat.h"


int main(int argc, const char * argv[]) {
    
//    std::string input_path = "/Users/maorshutman/repos/bipmat/inputs/edges_2x2_input_0.txt";
//    std::string input_path = "/Users/maorshutman/repos/bipmat/inputs/edges_2x2_input_1.txt";
//    std::string input_path = "/Users/maorshutman/repos/bipmat/inputs/edges_2x2_input_2.txt";
    std::string input_path = "/Users/maorshutman/repos/bipmat/inputs/matrix_3x3_input_0.txt";
    
//    wbm::BipartiteMatcher matcher = wbm::BipartiteMatcher(input_path, "edges");
    wbm::BipartiteMatcher matcher = wbm::BipartiteMatcher(input_path, "matrix");
        
    matcher.match();
    
    return 0;
}
