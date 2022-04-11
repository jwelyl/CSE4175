#include <iostream>
#include <string>
using namespace std;

string generator;

int main(void) {
  cin >> generator;

  while(generator[0] == '0')
    generator.erase(0, 1);

  cout << generator << "\n";
  cout << generator.size() << "\n";

  return 0;
}
