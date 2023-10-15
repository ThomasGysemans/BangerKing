#include <iostream>
#include "perf.hpp"
#include "../../include/token.hpp"
#include "../../include/lexer.hpp"
#include "../../include/parser.hpp"
#include "../../include/nodes/compositer.hpp"
#include "../../include/utils/deallocate_list_of_pointers.hpp"
#include "../../include/debug/print_tokens.hpp"
using namespace std;

class ParserPerformance: public Performance {
  public:
    ParserPerformance(unsigned int i): Performance(i) {}

    void test() const override {
      string code =
        "5+5\n"
        "5-5\n"
        "(5+5)\n"
        "10+2-10/2\n"
        "(5/5)%5**600000\n"
        "-1+2-(3*4)/5%6**7\n";
      Lexer lexer(code);
      list<Token*> tokens = lexer.generate_tokens();
      Parser parser(tokens);
      const ListNode* parsing_result = parser.parse();
      deallocate_list_of_pointers<Token>(tokens);
      delete parsing_result;
    }
};

int main() {
  ParserPerformance perf(100);
  double average_time = perf.run();
  cout << "Parser performance test did: " << std::to_string(average_time) << " ms." << endl;
  
  return 0;
}
