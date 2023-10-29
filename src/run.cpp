#include <iostream>
#include <list>
#include <map>
#include "../include/run.hpp"
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

const RuntimeResult* run(const string& input, const string& filename, const Context* ctx) {
  try {
    READ_FILES[filename] = &input;

    Lexer lexer(&input, filename);
    list<Token*> tokens = lexer.generate_tokens();

    if (tokens.empty()) {
      return nullptr;
    }

    Parser parser(tokens);
    const ListNode* tree = parser.parse();

    // The tokens are not necessary anymore so the memory can be released:
    deallocate_list_of_pointers<Token>(tokens);

    const RuntimeResult* result = Interpreter::visit(tree, ctx);

    delete tree;
    return result;
  } catch (RuntimeError e) {
    cerr << e.to_string() << endl;
  } catch (CustomError e) {
    cerr << e.to_string() << endl;
  } catch (Exception e) {
    cerr << e.to_string() << endl;
  } catch (...) {
    cerr << "Unhandled exception was triggered" << endl;
  }
  return nullptr;
}