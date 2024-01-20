#include <iostream>
#include <list>
#include <map>
#include "../include/run.hpp"
#include "../include/files.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/exceptions/custom_error.hpp"
#include "../include/exceptions/exception.hpp"
#include "../include/exceptions/runtime_error.hpp"
#include "../include/lexer.hpp"
#include "../include/token.hpp"
#include "../include/parser.hpp"
#include "../include/context.hpp"
#include "../include/runtime.hpp"
#include "../include/interpreter.hpp"
using namespace std;

unique_ptr<const RuntimeResult> run(const string& input, const string& filename, shared_ptr<Context> ctx) {
  try { 
    READ_FILES[filename] = make_unique<string>(input);

    Lexer lexer(&input, filename);
    const list<unique_ptr<const Token>> tokens = lexer.generate_tokens();

    if (tokens.empty()) {
      return nullptr;
    }

    Parser parser(tokens);
    unique_ptr<ListNode> tree = parser.parse();

    // The interpreter will progressively deallocate the nodes of the tree
    Interpreter::set_shared_ctx(ctx);
    unique_ptr<const RuntimeResult> result = Interpreter::visit(move(tree));

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