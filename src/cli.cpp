#include <iostream>
#include <list>
#include "../include/cli.hpp"
#include "../include/run.hpp"
#include "../include/context.hpp"
#include "../include/values/compositer.hpp"
#include "../include/runtime.hpp"
using namespace std;

void cli() {
  cout << "Welcome to Banger King !" << endl;
  cout << "Write your first program below." << endl;
  cout << "Type \\q to quit at any time." << endl;
  cout << endl;

  const shared_ptr<Context> ctx = make_shared<Context>("<cli>");

  string input;
  do {
    cout << ">> ";
    if (!getline(cin, input)) {
      break;
    }
    if (!input.starts_with("\\")) {
      unique_ptr<const RuntimeResult> res = runLine(input, ctx);
      if (res == nullptr) {
        continue;
      }
      const shared_ptr<ListValue> main_value = dynamic_pointer_cast<ListValue>(res->get_value());
      const list<shared_ptr<const Value>> values = main_value->get_elements();
      if (values.size() == 1) {
        cout << values.front()->to_string() << endl;
      } else {
        cout << main_value->to_string() << endl;
      }
      // The smart pointer "res" will take care of the memory deallocation.
      // It's not necessary to manually delete "main_value",
      // as the "res" variable is the actual owner.
      // It acts as an automatic garbage collector.
    }
  } while (input != "\\q");
}
