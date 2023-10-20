#include <iostream>
#include <list>
#include "helper.hpp"
#include "../include/utils/deallocate_list_of_pointers.hpp"
#include "../include/lexer.hpp"
#include "../include/token.hpp"
#include "../include/parser.hpp"
#include "../include/interpreter.hpp"
using namespace std;

/// @brief Gets the values that the interpreter returns from the given code. Use this for the tests.
/// @param code The code to interpret.
/// @return The values that the interpreter calculated.
const list<const Value*> get_values(const string& code) {
  try {
    Lexer lexer(code);
    list<Token*> tokens = lexer.generate_tokens();
    Parser parser(tokens);
    const ListNode* tree = parser.parse();
    deallocate_list_of_pointers<Token>(tokens);

    const Interpreter* interpreter = new Interpreter();
    const Context* ctx = new Context("<program>");
    const RuntimeResult* result = interpreter->visit(tree, *ctx);
    auto v = result->get_value();
    if (v == nullptr) {
      throw "Segmentation fault happened during interpretation of this code : " + code + " because the result is a `nullptr`.";
    }
    const ListValue* values = dynamic_cast<ListValue*>(result->get_value());
    if (values == nullptr) {
      throw "Invalid cast of interpreter result. It did not return a valid instance of ListValue.";
    }
    return *(values->get_elements());
  } catch (Exception e) {
    cout << "The program crashed due to this error: " << e.to_string() << endl;
    exit(1);
  }
}

void test_integer() {
  auto values = get_values("5");
  auto first_value = dynamic_cast<const IntegerValue*>(values.front());
  assert(first_value->get_actual_value() == 5);

  print_success_msg("works with integers", 1);
}

void test_addition_of_two_small_positive_integers() {
  auto values = get_values("5+5");
  auto first_value = dynamic_cast<const IntegerValue*>(values.front());
  assert(first_value->get_actual_value() == 10);

  print_success_msg("works with addition between two small integers", 1);
}

void test_addition_of_two_max_integers() {
  string half = std::to_string(INT_MAX / 2);
  auto values = get_values(half + " + " + half);
  auto first_value = dynamic_cast<const IntegerValue*>(values.front());
  assert(first_value->get_actual_value() == (INT_MAX - 1)); // (-1 because INT_MAX is not even, and so is rounded down)

  print_success_msg("works with addition between two max integers", 1);
}

int main() {
  print_title("Interpreter tests...");

  try {
    test_integer();
    test_addition_of_two_small_positive_integers();
    test_addition_of_two_max_integers();
  } catch (string cast_error) {
    cout << "ABORT. The tests crashed due to this error :" << endl;
    cout << cast_error << endl;
  }

  print_success_msg("All \"Interpreter\" tests successfully passed");
  return 0;
}