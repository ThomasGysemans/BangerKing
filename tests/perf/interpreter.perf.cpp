#include <iostream>
#include "perf.hpp"
#include "../../include/token.hpp"
#include "../../include/lexer.hpp"
#include "../../include/parser.hpp"
#include "../../include/interpreter.hpp"
#include "../../include/nodes/compositer.hpp"
#include "../../include/utils/deallocate_list_of_pointers.hpp"
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
      list<Token*> tokens = lexer.generate_tokens();
      Parser parser(tokens);
      const ListNode* tree = parser.parse();
      deallocate_list_of_pointers<Token>(tokens);

      Context* ctx = new Context("<program>");
      Interpreter::set_shared_ctx(ctx);
      const RuntimeResult* result = Interpreter::visit(tree);
      ListValue* main_value = dynamic_cast<ListValue*>(result->get_value());
      const list<const Value*>* values = main_value->get_elements();

      delete ctx;
      delete result;
      delete tree;
    }
};

int main() {
  InterpreterPerformance perf(100);
  perf.run_and_display("Interpreter performance test did: ");
  
  return 0;
}
