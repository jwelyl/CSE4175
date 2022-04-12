#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <bitset>
using namespace std;

FILE* fp_in = NULL;   //  input_file pointer
FILE* fp_out = NULL;  //  output_file pointer
FILE* fp_res = NULL;  //  result_file pointer
int dataword_size = 0;  //  하나의 dataword 크기(bit 수), 4 또는 8

string generator;       //  generator(divisor)
int generator_size = 0; //  generator(divisor)의 크기(bit 수)
char padding_size = 0;   //  padding bit 개수(0~7)를 저장하는 1byte character
int nbytes = 0;         //  padding bit 개수 나타내는 byte를 제외한 byte 수
string codewords = "";  //  padding bit를 제거한 codeword들을 bit로 저장한 문자열

void err_exit(const char* err_msg) {
  fprintf(stderr, "%s", err_msg);
  exit(-1);
} 

int main(int argc, char* argv[]) {
  char ch;  //  input_file에서 1byte씩 읽어오는 문자

  if(argc != 6) 
    err_exit("usage: ./crc_decoder input_file output_file result_file generator dataword_size\n");

  fp_in = fopen(argv[1], "r");
  if(!fp_in)
    err_exit("input file open error.\n");

  fp_out = fopen(argv[2], "w");
  if(!fp_out)
    err_exit("output file open error.\n");

  fp_res = fopen(argv[3], "w");
  if(!fp_res)
    err_exit("result file open error.\n");

  generator = string(argv[4]);
  dataword_size = atoi(argv[5]);

  ///
  cout << "generator = " << generator << "\n";
  ///

  if(dataword_size != 4 && dataword_size != 8)
    err_exit("dataword size must be 4 or 8.\n");

  fscanf(fp_in, "%c", &padding_size); //  첫 byte인 padding bit 개수 읽음

  if(!(0 <= padding_size && padding_size < 8))
    err_exit("padding bit error!\n");

  ///
  cout << "padding bits # : " << (int)padding_size << "\n";
  ///

  do {
    if(fscanf(fp_in, "%c", &ch) == EOF)
      break;

    bitset<8> b(ch);  //  1byte씩 문자를 읽어서 bitset으로 변환
    codewords += b.to_string();
  } while(true);

  codewords = codewords.substr((int)padding_size);
  cout << "codewords(padding bits 제거) : " << codewords << "\n";

  fclose(fp_in);
  fclose(fp_out);
  fclose(fp_res);

  return 0;
}
