#include <cstdio>
#include <iostream>
#include <bitset>
#include <cstdlib>
#include <string>
#include <cstring>
#include <deque>
using namespace std;

FILE* fp_in = NULL;       //  입력 파일 포인터
FILE* fp_out = NULL;      //  출력 파일 포인터
int dataword_size = 0;    //  하나의 dataword 크기(bit 수), 4 또는 8

string generator;         //  generator
int generator_size = 0;   //  generator(divisor)의 크기(bit 수)

int n_dataword = 0;       //  1byte(8-bit) data의 dataword 개수(2 또는 1)

int len_8 = 0;            //  codeword에 padding 0까지 추가한 길이(8의 배수)
char padding_size = 0;    //  output_file에 추가될 padding bit 개수(0~7) 

string codewords = "";  //  모든 dataword를 codeword로 변환 후 쭉 나열한 것

void err_exit(const char* err_msg) {
  fprintf(stderr, "%s", err_msg);
  exit(-1);
} 

//  calculate and return CRC remainder
string crc_remainder(string dataword);

//  write output_file
void write_output_file();

int main(int argc, char* argv[]) {
  if(argc != 5) 
    err_exit("usage: ./crc_encoder input_file output_file generator dataword_size\n");

  fp_in = fopen(argv[1], "r");
  if(!fp_in)
    err_exit("input file open error.\n");

  fp_out = fopen(argv[2], "w");
  if(!fp_out)
    err_exit("output file open error.\n");

  generator = string(argv[3]);
  while(generator[0] == '0')  //  최상위 bit가 1이 될 때 까지 0 제거
    generator.erase(0, 1);

  dataword_size = atoi(argv[4]);        
  if(dataword_size != 4 && dataword_size != 8)
    err_exit("dataword size must be 4 or 8.\n");

  generator_size = generator.size();
  n_dataword = (8 / dataword_size); //  dataword_size가 8일 경우, n_dataword = 1, 4일 경우, 2
  if(n_dataword != 1 && n_dataword != 2) {
    printf("n_dataword = %d\n", n_dataword);
    exit(-1);
  }

  if(dataword_size != 4 && dataword_size != 8)
    err_exit("dataword size must be 4 or 8.\n");
  
  //  입력 파일을 끝까지 1byte 단위로 읽음.
  do {
    char ch;  //  읽어온 1byte data
    
    if(fscanf(fp_in, "%c", &ch) == EOF) break;
    
    bitset<8> ch_bit(ch); //  읽어온 1byte를 8bits bitset으로 변환
    string ch_str = ch_bit.to_string(); //  8bits bitset을 문자열로 표현

    //  dataword_size만큼 1byte data에서 잘라냄
    for(int i = 0; i < n_dataword; i++) {
      string data_str = ch_str.substr(4 * i, dataword_size); //  dataword string
      string code_str = data_str; //  codeword string
      code_str += crc_remainder(data_str);
      codewords += code_str;
    } 
  } while(true);

  len_8 = codewords.size() + (8 - codewords.size() % 8);

  if(len_8 % 8 != 0) {
    cout << "wrong padding 0's\n";
    exit(-1);
  }
  padding_size = len_8 - codewords.size();

  codewords = string(padding_size, '0') + codewords;
  write_output_file();

  fclose(fp_in);
  fclose(fp_out);

  return 0;
}

string crc_remainder(string dataword) {
  deque<char> dividend; //  CRC 나눗셈의 피제수(dataword + ('generator 길이 - 1'개의 0)), O(1)에 앞에서 뺄 수 있도록 deque로 정의
  string remainder = ""; //  CRC 나눗셈의 나머지

  for(int i = 0; i < dataword.size(); i++)
    dividend.push_back(dataword[i]);

  for(int i = 0; i < generator_size - 1; i++)
    dividend.push_back('0');

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

  for(int i = 0; i < dividend.size(); i++)
    remainder.push_back(dividend[i]);
  
  return remainder;
}

void write_output_file() {
  int n_bytes = codewords.size() / 8; 
  fprintf(fp_out, "%c", padding_size);

  for(int i = 0; i < n_bytes; i++) {
    bitset<8> b(0);

    for(int j = 0; j < 8; j++)
      b[7 - j] = codewords[i * 8 + j] - '0';
    fprintf(fp_out, "%c", (char)b.to_ulong());
  }
}

