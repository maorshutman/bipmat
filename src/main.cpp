#include <iostream>
#include <unistd.h>

#include "bipmat.h"
#include "test.h"


#define TEST_MODE 1


void parse_args(int argc,
                char **argv,
                std::string &input_file_path,
                std::string &input_format)
{
  int c;
  
  while ((c = getopt(argc, argv, "i:f:oh")) != -1)
    switch (c) {
      case 'i':
        input_file_path = std::string(optarg);
        break;
        
      case 'f':
        input_format = std::string(optarg);
        break;
        
      case 'h':
        std::cout << "help string" << "\n";
        
        break;
        
      case 'o':
        std::cout << "output file path" << "\n";
        // if no file output to stdout ?
        break;
        
      case '?':
        return;
        
      default:
        // what ???
        abort();
      }
}


int main(int argc, char **argv)
{
#if(!TEST_MODE)
  std::string input_file_path;
  std::string input_format;
  parse_args(argc, argv, input_file_path, input_format);

  wbm::BipartiteMatcher* matcher = new wbm::BipartiteMatcher(input_file_path, input_format);
  matcher->match();
  
  // TODO: output.
  

  delete matcher;
  
#else
  
  test();

#endif
  
  return 0;
}
