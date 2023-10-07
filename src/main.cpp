#include <iostream>
#include "../include/run.h"
#include "../include/runtime_result.h"
using namespace std;

int main() {
  cout << "Welcome in Banger King !" << endl;
  cout << "Write your first program below." << endl;
  cout << "Type \\q to quit at any time." << endl;
  cout << endl;

  string input = "";
  while (input != "\\q") {
    cout << ">> ";
    cin >> input;
    if (!input.starts_with("\\")) {
      run(input, "<stdin>");
    }
  }

  return 0;
}
