#include <iostream>
#include <list>
#include <map>
#include "../include/files.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/utils/deallocate_list_of_pointers.hpp"
#include "../include/exceptions/custom_error.hpp"
#include "../include/exceptions/exception.hpp"
#include "../include/exceptions/runtime_error.hpp"
#include "../include/lexer.hpp"
#include "../include/parser.hpp"
#include "../include/token.hpp"
#include "../include/runtime.hpp"
#include "../include/context.hpp"
#include "../include/interpreter.hpp"
#include "../include/symbol_table.hpp"
using namespace std;

void run(const string& input, const string& filename, const Context* ctx) {
  try {

    READ_FILES[filename] = &input;

    Lexer lexer(input, filename);
    list<Token*> tokens = lexer.generate_tokens();

    if (tokens.empty()) {
      return;
    }

    Parser parser(tokens);
    const ListNode* tree = parser.parse();

    // The tokens are not necessary anymore so the memory can be released:
    deallocate_list_of_pointers<Token>(tokens);

    const Interpreter interpreter;
    const RuntimeResult* result = interpreter.visit(tree, ctx);
    ListValue* main_value = dynamic_cast<ListValue*>(result->get_value());
    const list<const Value*>* values = main_value->get_elements();

    if (values->size() == 1) {
      cout << values->front()->to_string() << endl;
    } else {
      cout << main_value->to_string() << endl;
    }

    delete result;
    delete tree;
  } catch (RuntimeError e) {
    cerr << e.to_string() << endl;
  } catch (CustomError e) {
    cerr << e.to_string() << endl;
  } catch (Exception e) {
    cerr << e.to_string() << endl;
  } catch (...) {
    cerr << "Unhandled exception was triggered" << endl;
  }
}