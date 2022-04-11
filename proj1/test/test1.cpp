#include <iostream>
#include <bitset>
using namespace std;

int main(void) {
  bitset<7> bit;
  bit.reset();

  for(int i = 6; i >= 0; i--) {
    char b;
    cin >> b;
   // cout << "b : " << b << "\n";
    bit[i] = b - '0';
   // cout << "bit[" << i << "] : " << bit[i] << "\n";
  }

  cout << "bit : " << bit << "\n";
  cout << "bit.to_string() : " << bit.to_string() << "\n";
  cout << "(int)bit.to_ulong() : " << (int)bit.to_ulong() << "\n";
  cout << "(char)bit.to_ulong() : " << (char)bit.to_ulong() << "\n";

  char ch;
  cout << "문자 하나 입력 : ";
  cin >> ch;

  bitset<8> ch_bit(ch);
  cout << "ch_bit : " << ch_bit << "\n";
  cout << "(int)ch_bit.to_ulong() : " << (int)ch_bit.to_ulong() << "\n";
  cout << "(char)ch_bit.to_ulong() : " << (char)ch_bit.to_ulong() << "\n";

  return 0;
}
