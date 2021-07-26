
#include "test.h"
#include "time.h"


// TODO: Add args.
void test()
{
    //    std::string out_dir =
  std::string out_filename = "/Users/maorshutman/repos/bipmat/tests/test_data.txt";
  
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
  
  std::vector<int> problem_size;
  std::vector<float> sol_time;
  Timer timer;
  int min_cost;
  
  for (int i = 0; i < input_file_paths.size(); i++) {
    std::cout << input_file_paths[i] << "\n";
    
    wbm::BipartiteMatcher matcher = wbm::BipartiteMatcher(input_file_paths[i], "edges");
    
    timer.reset();
    matcher.match();
    sol_time.push_back(timer.elapsed());
    
      // Read correct min cost.
    input = std::ifstream(output_file_paths[i]);
    getline(input, line);
    min_cost = stoi(line);
    
    std::cout << "correct = " << min_cost << ", " << matcher.get_min_cost() << std::endl;
    
    if (min_cost != matcher.get_min_cost()) {
      exit(1);
    }
    
    problem_size.push_back(matcher.get_n());
    
  }
  
  // Save to text file
  std::ofstream file;
  file.open(out_filename);
  for (int i = 0; i < input_file_paths.size(); i++) {
    file << problem_size[i] << ", " << sol_time[i] << "\n";
  }
  file.close();
  
  
  std::cout << "-> done" << "\n";
  
}
