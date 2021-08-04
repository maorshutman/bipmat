
#include <iostream>
#include <unistd.h>
#include <libgen.h>
#include "bipmat.h"
#include "test.h"

#define TEST_MODE 1


struct options_t {
  std::string input_file_path;
  std::string input_format;
  std::string output_file_path;
};

void parse_args(int argc, char **argv, options_t &opts);
void usage(char *prog_name, int opt);


int main(int argc, char **argv)
{
  
#if(!TEST_MODE)
  options_t opts;
  parse_args(argc, argv, opts);
  wbm::BipartiteMatcher* matcher = new wbm::BipartiteMatcher(opts.input_file_path, opts.input_format);
  matcher->match();
  matcher->dump_results(opts.output_file_path);
  delete matcher;
#else
  test();
#endif
  
  return 0;
}


void parse_args(int argc, char **argv, options_t &opts)
{
  int c;
  
  while ((c = getopt(argc, argv, "i:f:o:h")) != -1)
    switch (c) {
      case 'i':
        opts.input_file_path = std::string(optarg);
        break;
      case 'f':
        opts.input_format = std::string(optarg);
        break;
      case 'o':
        opts.output_file_path = std::string(optarg);
        break;
      case '?':
        std::cout << "Unknown option character: `" << char(optopt) << "`.\n";
        abort();
      case 'h':
      default:
        usage(basename(argv[0]), c);
    }
}


void usage(char *prog_name, int opt)
{
  printf("%s: Usage \n", prog_name);
  printf("\n");
  printf("  -i      : Input file path (required) \n");
  printf("  -f      : Server Port (required) \n");
  printf("  -o      : Output file path \n");
  printf("  -h      : Print this usage message \n");
  printf("\n");
}
