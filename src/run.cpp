#include <iostream>
#include <list>
#include <map>
#include "../include/run.hpp"
#include "../include/files.hpp"
#include "../include/exceptions/custom_error.hpp"
#include "../include/exceptions/exception.hpp"
#include "../include/parser.hpp"
#include "../include/context.hpp"
#include "../include/runtime.hpp"
#include "../include/interpreter.hpp"
using namespace std;

// To run the CLI:
unique_ptr<const RuntimeResult> runLine(const string& input, const shared_ptr<Context>& ctx) {
  if (input.empty()) {
    return nullptr;
  }

  try {
    READ_FILES["<stdin>"] = make_shared<string>(input);
    Parser parser = Parser::initCLI(input);
    unique_ptr<ListNode> tree = parser.parse();

    // The interpreter will progressively deallocate the nodes of the tree
    Interpreter::set_shared_ctx(ctx);
    unique_ptr<const RuntimeResult> result = Interpreter::visit(move(tree));

    return result;
  } catch (CustomError& e) {
    cerr << e.to_string() << endl;
  } catch (Exception& e) {
    cerr << e.to_string() << endl;
  }
  return nullptr;
}

// When reading a file, the Lexer will read the file character by character
// and construct a string containing the entire file (because the errors need it).
unique_ptr<const RuntimeResult> runFile(const string& path, const shared_ptr<Context>& ctx) {
  // I'm using a shared_ptr because I want READ_FILES to hold the value,
  // and I want the Lexer to be able to modify it.
  // I also want to use smart pointers for automatic memory management.
  const shared_ptr<string> source_code = make_shared<string>();

  try {
    READ_FILES[path] = source_code;
    Parser parser = Parser::initFile(source_code, path);
    unique_ptr<ListNode> tree = parser.parse();

    // The interpreter will progressively deallocate the nodes of the tree
    Interpreter::set_shared_ctx(ctx);
    unique_ptr<const RuntimeResult> result = Interpreter::visit(move(tree));

    return result;
  } catch (CustomError& e) {
    cerr << e.to_string() << endl;
  } catch (Exception& e) {
    cerr << e.to_string() << endl;
  }

  return nullptr;
}