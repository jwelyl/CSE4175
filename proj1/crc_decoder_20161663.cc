#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <bitset>
#include <deque>
using namespace std;

FILE* fp_in = NULL;   //  input_file pointer
FILE* fp_out = NULL;  //  output_file pointer
FILE* fp_res = NULL;  //  result_file pointer
int dataword_size = 0;  //  하나의 dataword 크기(bit 수), 4 또는 8

string generator;       //  generator(divisor)
int generator_size = 0; //  generator(divisor)의 크기(bit 수)
char padding_size = 0;   //  padding bit 개수(0~7)를 저장하는 1byte character
string codewords = "";  //  padding bit를 제거한 codeword들을 bit로 저장한 문자열
int l_codeword = 0;   //  codeword 길이
int n_codeword = 0;   //  총 codeword 개수(dataword의 개수이기도 함)
int n_error = 0;      //  error 발생한 codeword 개수
string datawords = "";  //  output_file에 복원할 dataword

void err_exit(const char* err_msg); //  error message 출력 후 프로그램 종료
void crc_receive(string codeword);  //  codeword가 error가 있는지 확인 
bool is_error(deque<char> remainder);  //  error가 확실하면 true, 아니면 false
void write_files(); //  output_file, result_file 작성

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
  generator_size = generator.size();
  dataword_size = atoi(argv[5]);

  if(dataword_size != 4 && dataword_size != 8)
    err_exit("dataword size must be 4 or 8.\n");

  fscanf(fp_in, "%c", &padding_size); //  첫 byte인 padding bit 개수 읽음

  if(!(0 <= padding_size && padding_size < 8))
    err_exit("padding bit error!\n");

  do {
    if(fscanf(fp_in, "%c", &ch) == EOF)
      break;

    bitset<8> b(ch);  //  1byte씩 문자를 읽어서 bitset으로 변환
    codewords += b.to_string();
  } while(true);

  codewords = codewords.substr((int)padding_size);
  l_codeword = dataword_size + generator_size - 1;
  n_codeword = codewords.size() / l_codeword;

  for(int i = 0; i < n_codeword; i++) {
    string codeword = codewords.substr(i * l_codeword, l_codeword);
    crc_receive(codeword);
  }

  write_files();

  fclose(fp_in);
  fclose(fp_out);
  fclose(fp_res);

  return 0;
}

void err_exit(const char* err_msg) {
  fprintf(stderr, "%s", err_msg);
  exit(-1);
}

void crc_receive(string codeword) {
  deque<char> dividend; //  CRC 나눗셈의 피제수(codeword)
  string dataword = codeword; //  복원해낸 dataword

  for(int i = 0; i < codeword.size(); i++)
    dividend.push_back(dataword[i]);

  for(int i = 0; i < generator_size - 1; i++) //  codeword의 끝에서 generator_size - 1 개 bit 제거하여 dataword 복원
    dataword.pop_back();

  while(dividend.size() >= generator_size) {
    string tmp = generator; //  몫

    if(dividend[0] == '0') {  //  몫이 0일 경우
      for(int i = 0; i < tmp.size(); i++)
        tmp[i] = '0';
    }

    for(int i = 0; i < tmp.size(); i++) { //  XOR 연산
      if(dividend[i] == tmp[i])
        dividend[i] = '0';
      else
        dividend[i] = '1';
    }

    dividend.pop_front();
  }

  if(is_error(dividend))  //  error일 경우
    n_error += 1;

  //  error 여부에 관계 없이 dataword 복원
  datawords += dataword;
}

bool is_error(deque<char> remainder) {
  //  remainder의 bit가 모두 0일 때 error 없음, 하나라도 1이 존재할 경우 error
  for(int i = 0; i < remainder.size(); i++) {
    if(remainder[i] == '1')
      return true;
  }

  return false;
}

void write_files() {
  int nbytes = datawords.size() / 8;  //  datawords의 총 byte 수

  //  dataword 복원하여 output_file에 작성
  for(int i = 0; i < nbytes; i++) {
    string tmp = datawords.substr(i * 8, 8);
    bitset<8> b(tmp);
    
    fprintf(fp_out, "%c", (char)b.to_ulong());
  }

  //  result_file 작성
  fprintf(fp_res, "%d %d", n_codeword, n_error);
}
