#include <iostream>
#include <list>
#include <map>
#include "../include/miscellaneous.hpp"
#include "../include/utils/deallocate_list_of_pointers.hpp"
#include "../include/exceptions/custom_error.hpp"
#include "../include/exceptions/exception.hpp"
#include "../include/debug/print_tokens.hpp"
#include "../include/lexer.hpp"
#include "../include/parser.hpp"
#include "../include/token.hpp"
#include "../include/files.hpp"
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
    // cout << display_tokens_list(tokens) << endl;

    if (tokens.empty()) {
      return;
    }

    Parser parser(tokens);
    const ListNode* tree = parser.parse();

    // cout << tree->to_string() << endl;

    // The tokens are not necessary anymore so the memory can be released:
    deallocate_list_of_pointers<Token>(tokens);

    const Interpreter* interpreter = new Interpreter();
    const RuntimeResult* result = interpreter->visit(tree, *ctx);
    ListValue* main_value = dynamic_cast<ListValue*>(result->get_value());
    const list<const Value*>* values = main_value->get_elements();

    if (values->size() == 1) {
      cout << values->front()->to_string() << endl;
    } else {
      cout << main_value->to_string() << endl;
    }

    // cout << "Interpretation is done" << endl;

    delete main_value; // also deletes "values"
    delete result;
    delete interpreter;
    delete tree;
  } catch (CustomError e) {
    cerr << "The program crashed !!" << endl;
    cerr << e.to_string() << endl;
  } catch (Exception e) {
    cerr << "The language itself crashed due to this Exception:" << endl;
    cerr << e.to_string() << endl;
  }
}