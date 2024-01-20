#include <iostream>
#include "perf.hpp"
#include "../../include/token.hpp"
#include "../../include/lexer.hpp"
#include "../../include/parser.hpp"
#include "../../include/interpreter.hpp"
#include "../../include/nodes/compositer.hpp"
#include "../../include/debug/print_tokens.hpp"
using namespace std;

class InterpreterPerformance: public Performance {
  public:
    InterpreterPerformance(unsigned int i): Performance(i) {}

    void test() const override {
      string code =
        "5+5\n"
        "5-5\n"
        "(5+5)\n"
        "10+2-10/2\n"
        "(5/5)%5**600000\n"
        "-1+2-(3*4)/5%6**7\n";
      Lexer lexer(&code);
      list<unique_ptr<const Token>> tokens = lexer.generate_tokens();
      Parser parser(tokens);
      unique_ptr<ListNode> tree = parser.parse();

      shared_ptr<Context> ctx = make_shared<Context>("<perf>");
      Interpreter::set_shared_ctx(ctx);
      unique_ptr<const RuntimeResult> result = Interpreter::visit(move(tree));
    }
};

int main() {
  InterpreterPerformance perf(100);
  perf.run_and_display("Interpreter performance test did: ");
  
  return 0;
}
