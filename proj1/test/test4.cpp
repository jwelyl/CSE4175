#include <cstdio>
#include <iostream>
#include <bitset>
using namespace std;

int main(void) {
  FILE* fp = fopen("codedstream.tx", "r");
  char ch;

  while(fscanf(fp, "%c", &ch) != EOF) {
    bitset<8> b(ch);
    cout << b << " (" << (int)b.to_ulong() << ")\n";
  }

  return 0;
}
