#include <iostream>
#include "../include/run.hpp"
using namespace std;

int main() {
  cout << "Welcome to Banger King !" << endl;
  cout << "Write your first program below." << endl;
  cout << "Type \\q to quit at any time." << endl;
  cout << endl;

  string input = "";
  while (input != "\\q") {
    cout << ">> ";
    getline(cin, input);
    if (!input.starts_with("\\")) {
      run(input, "<stdin>");
    }
  }

  return 0;
}
