#include <iostream>
#include <unistd.h>

#include "bipmat.h"
#include "test.h"



  // TODO:
  // add cmd args standard way
  // run using the binary



int main(int argc, const char * argv[]) {
  
    // --i,
    // --ifmt //edges or matrix
    // --t if want to run tests, or keep internal
    // -h
  
  int run_test = 1;
  
  if (!run_test)
  {
    std::string input_path = "/Users/maorshutman/repos/bipmat/tests/inputs/edges_1020x1020_input_0.txt";
    wbm::BipartiteMatcher* matcher = new wbm::BipartiteMatcher(input_path, "edges");
    matcher->match();
    std::cout << "min cost " << matcher->get_min_cost() << std::endl;
    delete matcher;
  }
  else
  {
    test();
  }
  
  return 0;
}
