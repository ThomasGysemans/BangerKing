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
using namespace std;

void run(const string& input, const string& filename) {
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
    const Context* ctx = new Context("<program>");
    const RuntimeResult* result = interpreter->visit(tree, *ctx);
    ListValue* main_value = dynamic_cast<ListValue*>(result->get_value());
    const list<const Value*>* values = main_value->get_elements();

    if (values->size() == 1) {
      cout << values->front()->to_string() << endl;
    } else {
      cout << main_value->to_string() << endl;
    }

    // cout << "Interpretation is done" << endl;

    // these variables are not necessary anymore
    delete main_value;
    delete result;
    delete ctx;
    delete interpreter;
    delete tree;
  } catch (CustomError e) {
    cout << "The program crashed !!" << endl;
    cout << e.to_string() << endl;
  } catch (Exception e) {
    cout << "The language itself crashed due to this Exception:" << endl;
    cout << e.to_string() << endl;
  }
}