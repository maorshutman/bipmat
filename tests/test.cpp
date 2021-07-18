
#include "test.h"


void test()
{
    std::string input_files_list = "/Users/maorshutman/repos/bipmat/tests/inputs/inputs_file_list.txt";
    std::string output_files_list = "/Users/maorshutman/repos/bipmat/tests/outputs/outputs_file_list.txt";
    std::vector<std::string> input_file_paths;
    std::vector<std::string> output_file_paths;
    
    std::string line;
    std::ifstream input;

    input = std::ifstream(input_files_list);
    while (getline(input, line)) {
        input_file_paths.push_back(line);
    }
    
    input = std::ifstream(output_files_list);
    while (getline(input, line)) {
        output_file_paths.push_back(line);
    }
    
    int min_cost;
    
    for (int i = 0; i < input_file_paths.size(); i++) {
        std::cout << input_file_paths[i] << "\n";
        
        wbm::BipartiteMatcher matcher = wbm::BipartiteMatcher(input_file_paths[i], "edges");
        matcher.match();
                
        // Read correct min cost.
        input = std::ifstream(output_file_paths[i]);
        getline(input, line);
        min_cost = stoi(line);

        std::cout << "correct = " << min_cost << ", " << matcher.get_min_cost() << std::endl;
        
    }
    
    std::cout << "-> done" << "\n";
    
}
