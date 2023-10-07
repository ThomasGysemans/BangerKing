#include <iostream>
#include <list>
#include "../include/run.h"
#include "../include/exceptions/custom_error.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/token.h"
#include "../include/utils/deallocate_list_of_pointers.h"
#include "../include/debug/print_tokens.h"
using namespace std;

void run(string input, string filename) {
  try {
    Lexer lexer(input, filename);
    list<Token*> tokens = lexer.generate_tokens();
    cout << display_tokens_list(tokens) << endl;

    if (tokens.empty()) {
      return;
    }

    Parser parser(tokens);
    const ListNode * parsing_result = parser.parse();

    cout << parsing_result->to_string() << endl;

    // The tokens are not necessary anymore so the memory can be released:
    deallocate_list_of_pointers<Token>(tokens);

    // the nodes are not necessary anymore so the memory can be released:
    delete parsing_result;
  } catch (CustomError e) {
    cout << "The program crashed !!" << endl;
    cout << e.to_string() << endl;
  }
}