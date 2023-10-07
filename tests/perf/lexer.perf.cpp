#include <iostream>
#include "perf.h"
#include "../../include/token.h"
#include "../../include/lexer.h"
#include "../../include/utils/deallocate_list_of_pointers.h"
#include "../../include/debug/print_tokens.h"
using namespace std;

class LexerPerformance: public Performance {
  public:
    LexerPerformance(unsigned int i): Performance(i) {}

    void test() const override {
      string code =
        "IDENTIFIER "
        "5+5-5 "
        "5++ "
        "5-- "
        "(5/5)%8**6000000 "
        "\n ";
      code += KEYWORDS[0];
      Lexer lexer(code);
      list<Token*> tokens = lexer.generate_tokens();
      deallocate_list_of_pointers<Token>(tokens);
    }
};

int main() {
  LexerPerformance perf(100);
  double average_time = perf.run();
  cout << "Lexer performance test did: " << std::to_string(average_time) << " ms." << endl;
  
  return 0;
}
