#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
using namespace std;

void err_exit(const char* err_msg) {
  fprintf(stderr, "%s", err_msg);
  exit(-1);
} 

int main(int argc, char* argv[]) {
  if(argc != 6) 
    err_exit("usage: ./crc_decoder input_file output_file result_file generator dataword_size\n");

  FILE* fp_in = fopen(argv[1], "r");
  if(!fp_in)
    err_exit("input file open error.\n");

  FILE* fp_out = fopen(argv[2], "w");
  if(!fp_out)
    err_exit("output file open error.\n");

  FILE* fp_res = fopen(argv[3], "w");
  if(!fp_res)
    err_exit("result file open error.\n");

  string generator = string(argv[3]);
  int dataword_size = atoi(argv[4]);

  cout << "generator = " << generator << "\n";

  if(dataword_size != 4 && dataword_size != 8)
    err_exit("dataword size must be 4 or 8.\n");


  return 0;
}
