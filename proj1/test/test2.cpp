#include <iostream>
#include <string>
#include <deque>
using namespace std;

deque<char> dq;
string str;

int main(void) {
  str = "01011100";
  cout << "str : " << str << "\n";
  for(int i = 0; i < str.size(); i++) {
    dq.push_back(str[i]);
  }

  cout << "dq : ";
  for(int i = 0; i < dq.size(); i++)
    cout << dq[i];
  cout << "\n";

  return 0;
}
