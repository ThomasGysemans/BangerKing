#include <iostream>
#include <list>
#include "helper.hpp"
#include "../include/utils/deallocate_list_of_pointers.hpp"
#include "../include/lexer.hpp"
#include "../include/token.hpp"
#include "../include/parser.hpp"
#include "../include/interpreter.hpp"
#include "../include/symbol_table.hpp"
#include "../include/values/compositer.hpp"
#include "../include/exceptions/runtime_error.hpp"
using namespace std;

Context* common_ctx = new Context("<tests>");

/// @brief Gets the values that the interpreter returns from the given code. Use this for the tests.
/// @param code The code to interpret.
/// @return The values that the interpreter calculated.
const list<const Value*> get_values(const string& code, bool clear_ctx = true) {
  if (clear_ctx) common_ctx->get_symbol_table()->clear();

  try {
    Lexer lexer(&code);
    list<Token*> tokens = lexer.generate_tokens();
    Parser parser(tokens);
    const ListNode* tree = parser.parse();
    deallocate_list_of_pointers<Token>(tokens);

    const Interpreter interpreter;
    const RuntimeResult* result = interpreter.visit(tree, common_ctx);
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
    cerr << "The program crashed due to this error: " << e.to_string() << endl;
    exit(1);
  }
}

void execute(const string& code) {
  Lexer lexer(&code);
  list<Token*> tokens = lexer.generate_tokens();
  Parser parser(tokens);
  const ListNode* tree = parser.parse();
  deallocate_list_of_pointers<Token>(tokens);
  
  const Interpreter* interpreter = new Interpreter();
  interpreter->visit(tree, common_ctx);

  delete interpreter;
}

void test_empty_input() {
  auto values = get_values("\n\n");
  assert(values.empty());

  print_success_msg("works with empty input", 1);
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

void test_substraction_of_two_small_positive_integers() {
  auto values = get_values("5-3");
  auto first_value = dynamic_cast<const IntegerValue*>(values.front());
  assert(first_value->get_actual_value() == 2);

  print_success_msg("works with substraction between two small integers", 1);
}

void test_substraction_of_two_small_positive_integers_with_whitespace() {
  auto values = get_values("5 - 3");
  auto first_value = dynamic_cast<const IntegerValue*>(values.front());
  assert(first_value->get_actual_value() == 2);
  assert(first_value->to_string() == "2");

  print_success_msg("works with substraction between two small integers with whitespace", 1);
}

void test_multiplication_with_integers() {
  auto values = get_values("5 * 3");
  auto first_value = dynamic_cast<const IntegerValue*>(values.front());
  assert(first_value->get_actual_value() == 15);
  assert(first_value->to_string() == "15");

  print_success_msg("works with multiplication of integers", 1);
}

void test_power_with_integers() {
  auto values = get_values("5 ** 3");
  auto first_value = dynamic_cast<const IntegerValue*>(values.front());
  assert(first_value->get_actual_value() == 125);

  print_success_msg("works with power of integers", 1);
}

void test_divide_with_integers() {
  auto values = get_values("10 / 2");
  auto first_value = dynamic_cast<const IntegerValue*>(values.front());
  assert(first_value->get_actual_value() == 5);

  print_success_msg("works with division of integers", 1);
}

void test_modulo_with_integers() {
  auto values = get_values("12 % 10");
  auto first_value = dynamic_cast<const IntegerValue*>(values.front());
  assert(first_value->get_actual_value() == 2);

  print_success_msg("works with modulo of integers", 1);
}

void test_mathematical_operation_without_parenthesis() {
  auto values = get_values("5 * 2 -2 / 2");
  auto first_value = dynamic_cast<const IntegerValue*>(values.front());
  assert(first_value->get_actual_value() == 9);

  print_success_msg("works with a mathematical operation without parenthesis", 1);
}

void test_mathematical_operation_with_parenthesis() {
  auto values = get_values("5 * (2 - 2) / 2");
  auto first_value = dynamic_cast<const IntegerValue*>(values.front());
  assert(first_value->get_actual_value() == 0);

  print_success_msg("works with a mathematical operation with parenthesis", 1);
}

void test_negative_integer() {
  auto values = get_values("-5");
  auto first_value = dynamic_cast<const IntegerValue*>(values.front());
  assert(first_value->get_actual_value() == -5);

  print_success_msg("works with a negative value", 1);
}

void test_positive_integer() {
  auto values = get_values("+5");
  auto first_value = dynamic_cast<const IntegerValue*>(values.front());
  assert(first_value->get_actual_value() == 5);

  print_success_msg("works with a positive value", 1);
}

void test_variable_assignment_of_an_integer() {
  auto values = get_values("store a as int = 5");
  auto first_value = dynamic_cast<const IntegerValue*>(values.front());
  assert(first_value->get_actual_value() == 5);
  assert(common_ctx->get_symbol_table()->exists("a"));

  auto v = common_ctx->get_symbol_table()->get("a");
  auto i = dynamic_cast<IntegerValue*>(v);
  assert(instanceof<IntegerValue>(v));

  print_success_msg("works with the variable assignment of an integer", 1);
}

void test_variable_assignment_of_an_integer_without_initial_value() {
  auto values = get_values("store a as int");
  auto first_value = dynamic_cast<const IntegerValue*>(values.front());
  assert(first_value->get_actual_value() == 0);
  assert(common_ctx->get_symbol_table()->exists("a"));

  print_success_msg("works with the variable assignment of an integer without an initial value", 1);
}

void test_redefinition_of_existing_variable() {
  common_ctx->get_symbol_table()->clear();
  try {
    execute("store a as int = 4");
    execute("store a as int = 1");
    assert(false);
  } catch (RuntimeError error) {
    assert(true);
    print_success_msg("throws an error when declaring multiple times the same variable", 1);
  } catch (...) {
    assert(false);
  }
}

void test_access_to_variable() {
  try {
    common_ctx->get_symbol_table()->clear();
    execute("store a as int = 5");
    auto values = get_values("a", false);
    auto first_value = dynamic_cast<const IntegerValue*>(values.front());
    assert(first_value->get_actual_value() == 5);
    assert(common_ctx->get_symbol_table()->exists("a"));
    
    print_success_msg("works the access to a variable", 1);
  } catch (RuntimeError e) {
    cout << e.to_string() << endl;
    assert(false);
  }
}

void test_access_to_variable_in_maths_expression() {
  try {
    common_ctx->get_symbol_table()->clear();
    execute("store a as int = 5");
    execute("a"); // use it at least once and then test it
    auto values = get_values("a+5", false);
    auto first_value = dynamic_cast<const IntegerValue*>(values.front());
    assert(first_value->get_actual_value() == 10);
    assert(common_ctx->get_symbol_table()->exists("a"));

    IntegerValue* value_from_table = dynamic_cast<IntegerValue*>(common_ctx->get_symbol_table()->get("a"));
    // the program returns a copy of the value set in the table
    // because it gets deallocated by the garbage collector of 'run()'
    assert(&value_from_table != &first_value); 

    print_success_msg("works with the access to a variable in maths expression", 1);
  } catch (RuntimeError e) {
    cout << e.to_string() << endl;
    assert(false);
  }
}

void test_multiline_input() {
  auto values = get_values("5+5\n6+7\n");
  assert(values.size() == 2);
  assert(instanceof<IntegerValue>(values.front()));
  assert(instanceof<IntegerValue>(values.back()));
  auto first = dynamic_cast<const IntegerValue*>(values.front());
  auto second = dynamic_cast<const IntegerValue*>(values.back());
  assert(first->get_actual_value() == 10);
  assert(second->get_actual_value() == 13);

  print_success_msg("works with multiple lines", 1);
}

void test_multiline_input_and_variables() {
  // test using variables
  auto values = get_values("store a as int = 5\na\n");
  assert(values.size() == 2);
  assert(instanceof<IntegerValue>(values.front()));
  assert(instanceof<IntegerValue>(values.back()));
  auto first = dynamic_cast<const IntegerValue*>(values.front());
  auto second = dynamic_cast<const IntegerValue*>(values.back());
  assert(first->get_actual_value() == 5);
  assert(second->get_actual_value() == 5);
  
  print_success_msg("works with variables on multiple lines", 1);
}

int main() {
  print_title("Interpreter tests...");

  try {
    test_empty_input();
    test_integer();
    test_addition_of_two_small_positive_integers();
    test_addition_of_two_max_integers();
    test_substraction_of_two_small_positive_integers();
    test_substraction_of_two_small_positive_integers_with_whitespace();
    test_multiplication_with_integers();
    test_power_with_integers();
    test_divide_with_integers();
    test_modulo_with_integers();
    test_mathematical_operation_without_parenthesis();
    test_mathematical_operation_with_parenthesis();
    test_negative_integer();
    test_positive_integer();
    test_variable_assignment_of_an_integer();
    test_variable_assignment_of_an_integer_without_initial_value();
    test_redefinition_of_existing_variable();
    test_access_to_variable();
    test_access_to_variable_in_maths_expression();
    test_multiline_input();
    test_multiline_input_and_variables();
  } catch (string cast_error) {
    cerr << "ABORT. The tests crashed due to this error :" << endl;
    cerr << cast_error << endl;
  }

  print_success_msg("All \"Interpreter\" tests successfully passed");
  return 0;
}