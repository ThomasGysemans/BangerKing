#include <iostream>
#include <list>
#include "../include/run.hpp"
#include "../include/context.hpp"
#include "../include/values/compositer.hpp"
#include "../include/runtime.hpp"
using namespace std;

int main() {
  cout << "Welcome to Banger King !" << endl;
  cout << "Write your first program below." << endl;
  cout << "Type \\q to quit at any time." << endl;
  cout << endl;

  Context* ctx = new Context("<cli>");

  string input = "";
  while (input != "\\q") {
    cout << ">> ";
    getline(cin, input);
    if (!input.starts_with("\\")) {
      const RuntimeResult* res = run(input, "<stdin>", ctx);
      if (res == nullptr) {
        continue;
      }
      ListValue* main_value = dynamic_cast<ListValue*>(res->get_value());
      const list<const Value*>* values = main_value->get_elements();
      if (values->size() == 1) {
        cout << values->front()->to_string() << endl;
      } else {
        cout << main_value->to_string() << endl;
      }
      delete res; // garbage collector (it will delete "main_value" and "values" altogether)
    }
  }

  delete ctx;

  return 0;
}
