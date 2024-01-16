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
#include "../include/exceptions/type_error.hpp"
#include "../include/types.hpp"
using namespace std;

Context* common_ctx = new Context("<tests>");

/// @brief Gets the values that the interpreter returns from the given code. Use this for the tests.
/// @param code The code to interpret.
/// @param clear_ctx Whether or not the global context should be cleared before interpreting the given source code. Defaults to `true`.
/// @return The values that the interpreter calculated.
const list<const Value*> get_values(const string& code, bool clear_ctx = true) {
  if (clear_ctx) common_ctx->get_symbol_table()->clear();

  try {
    Lexer lexer(&code);
    list<Token*> tokens = lexer.generate_tokens();
    Parser parser(tokens);
    const ListNode* tree = parser.parse();
    deallocate_list_of_pointers<Token>(tokens);

    Interpreter::set_shared_ctx(common_ctx);
    const RuntimeResult* result = Interpreter::visit(tree);
    Value* v = result->get_value();
    if (v == nullptr) {
      throw "Segmentation fault happened during interpretation of this code : " + code + " because the result is a `nullptr`.";
    }
    const ListValue* values = dynamic_cast<ListValue*>(v);
    if (values == nullptr) {
      throw "Invalid cast of interpreter result. It did not return a valid instance of ListValue.";
    }
    delete tree;
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
  
  Interpreter::set_shared_ctx(common_ctx);
  auto res = Interpreter::visit(tree);
  delete res;
  delete tree;
}

template <typename V>
unique_ptr<const V> get_custom_value_from(const string& code, bool clear_ctx = true) {
  return make_unique<const V>(*(dynamic_cast<const V*>(get_values(code, clear_ctx).front())));
}

template <typename V, typename N>
N get_actual_value_from(const string& code, bool clear_ctx = true) {
  return make_unique<const V>(*(dynamic_cast<const V*>(get_values(code, clear_ctx).front())))->get_actual_value();
}

template <typename V, typename N>
bool compare_actual_value(const string& code, const N& expected, bool clear_ctx = true) {
  return get_actual_value_from<V, N>(code, clear_ctx) == expected;
}

void test_empty_input() {
  const auto values = get_values("\n\n");
  assert(values.empty());

  print_success_msg("works with empty input", 1);
}

void test_integer() {
  assert(compare_actual_value<IntegerValue>("5", 5));
  assert(compare_actual_value<IntegerValue>("+5", 5));
  assert(compare_actual_value<IntegerValue>("-5", -5));

  assert(compare_actual_value<IntegerValue>("5+2", 7));
  assert(compare_actual_value<IntegerValue>("10-5", 5));
  assert(compare_actual_value<IntegerValue>("2*2", 4));
  assert(compare_actual_value<IntegerValue>("10**2", 100));
  assert(compare_actual_value<IntegerValue>("10**2", 100));
  assert(compare_actual_value<IntegerValue>("10/3", 3));
  assert(compare_actual_value<IntegerValue>("10%2", 0));

  assert(compare_actual_value<IntegerValue>("5+-2", 3));
  assert(compare_actual_value<IntegerValue>("-10-5", -15));
  assert(compare_actual_value<IntegerValue>("-2*2", -4));
  assert(compare_actual_value<IntegerValue>("-10**2", 100));
  assert(compare_actual_value<IntegerValue>("-10/3", -3));
  assert(compare_actual_value<IntegerValue>("-10%2", 0));
  assert(compare_actual_value<IntegerValue>("10%-3", 1));
  assert(compare_actual_value<IntegerValue>("-10%-3", -1));
  assert(compare_actual_value<IntegerValue>("-10%3", -1)); // the first operand defines the sign of the result

  print_success_msg("works with positive and negative integers, and works with operations between integers", 1);
}

void test_double() {
  assert(compare_actual_value<DoubleValue>("3.14", 3.14));
  assert(compare_actual_value<DoubleValue>("+3.14", 3.14));
  assert(compare_actual_value<DoubleValue>("-3.14", -3.14));

  // Additions
  assert(compare_actual_value<DoubleValue>("1+0.2", 1.2));
  assert(compare_actual_value<DoubleValue>("0.1+1", 1.1));
  assert((get_actual_value_from<DoubleValue, double>("0.1+0.2")) != 0.3); // when using doubles, 0.1+0.2 != 0.3
  assert(get_custom_value_from<DoubleValue>("0.1+0.2")->to_string() == "0.3"); // however it should still print the right value
  assert(compare_actual_value<DoubleValue>("5.0+-1", 4.0));
  // Substraction
  assert(compare_actual_value<DoubleValue>("1-0.5", 0.5));
  assert(compare_actual_value<DoubleValue>("1.5-0.5", 1.0));
  assert(compare_actual_value<DoubleValue>("0.5-0.5", 0.0));
  assert(compare_actual_value<DoubleValue>("0.5- -0.5", 1.0)); // a whitespace is needed because '--' is the decrement operator
  // Multiplications
  assert(compare_actual_value<DoubleValue>("2.2*2", 4.4));
  assert(compare_actual_value<DoubleValue>("2*2.5", 5.0));
  assert(compare_actual_value<DoubleValue>("2.5*0.1", 0.25));
  assert(compare_actual_value<DoubleValue>("2.5*-0.1", -0.25));
  // Power
  assert(compare_actual_value<DoubleValue>("10**2.0", 100.0));
  assert(compare_actual_value<DoubleValue>("10.0**2", 100.0));
  assert(compare_actual_value<DoubleValue>("10.0**2.0", 100.0));
  assert(compare_actual_value<DoubleValue>("10.0**-2.0", 0.01));
  // Divisions
  assert(compare_actual_value<DoubleValue>("10/2.0", 5.0));
  assert(compare_actual_value<DoubleValue>("2.0/2", 1.0));
  assert(compare_actual_value<DoubleValue>("5.0/2.0", 2.5));
  assert(compare_actual_value<DoubleValue>("5.0/-2.0", -2.5));
  // Modulo
  assert(compare_actual_value<DoubleValue>("10%2.0", 0.0));
  assert(compare_actual_value<DoubleValue>("2.0%2", 0.0));
  assert(compare_actual_value<DoubleValue>("5.0%2.0", 1.0));
  assert(compare_actual_value<DoubleValue>("5.0%-2.0", 1));
  assert(compare_actual_value<DoubleValue>("-5.0%-2.0", -1));
  assert(compare_actual_value<DoubleValue>("-5.0%2.0", -1)); // the first operand defines the sign of the result

  print_success_msg("works with positive and negative doubles, and works with operations involving doubles", 1);
}

void test_addition_of_two_max_integers() {
  const string half = std::to_string(INT_MAX / 2);
  assert(compare_actual_value<IntegerValue>(half + " + " + half, (INT_MAX - 1))); // (-1 because INT_MAX is not even, and so is rounded down)
  print_success_msg("works with addition between two max integers", 1);
}

void test_mathematical_operation_without_parenthesis() {
  assert(compare_actual_value<IntegerValue>("5 * 2 -2 / 2", 9));
  print_success_msg("works with a mathematical operation without parenthesis", 1);
}

void test_mathematical_operation_with_parenthesis() {
  assert(compare_actual_value<IntegerValue>("5 * (2 - 2) / 2", 0));
  print_success_msg("works with a mathematical operation with parenthesis", 1);
}

void test_mathematical_operation_with_negative_and_double_numbers() {
  assert(compare_actual_value<DoubleValue>("(-2.5 * 2 / -5.0) * 10 ** 2", 100));
  print_success_msg("works with a complex mathematical operation with negative numbers and doubles", 1);
}

void test_variable_assignment_of_an_integer() {
  assert(compare_actual_value<IntegerValue>("store a as int = 5", 5));
  assert(common_ctx->get_symbol_table()->exists("a"));
  assert(instanceof<IntegerValue>(common_ctx->get_symbol_table()->get("a")));

  assert(compare_actual_value<IntegerValue>("store b as int", IntegerValue::get_default_value()));
  assert(common_ctx->get_symbol_table()->exists("b"));
  assert(instanceof<IntegerValue>(common_ctx->get_symbol_table()->get("b")));

  assert(compare_actual_value<IntegerValue>("store c as int = 5.6", 5));
  assert(common_ctx->get_symbol_table()->exists("c"));
  assert(instanceof<IntegerValue>(common_ctx->get_symbol_table()->get("c")));

  print_success_msg("works with the variable assignment of an integer", 1);
}

void test_variable_assignment_of_a_double() {
  assert(compare_actual_value<DoubleValue>("store a as double = 5", 5));
  assert(common_ctx->get_symbol_table()->exists("a"));
  assert(instanceof<DoubleValue>(common_ctx->get_symbol_table()->get("a")));

  assert(compare_actual_value<DoubleValue>("store b as double = 5.5", 5.5));
  assert(common_ctx->get_symbol_table()->exists("b"));
  assert(instanceof<DoubleValue>(common_ctx->get_symbol_table()->get("b")));

  assert(compare_actual_value<DoubleValue>("store c as double", DoubleValue::get_default_value()));
  assert(common_ctx->get_symbol_table()->exists("c"));
  assert(instanceof<DoubleValue>(common_ctx->get_symbol_table()->get("c")));

  print_success_msg("works with the variable assignment of a double", 1);
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
    assert(compare_actual_value<IntegerValue>("a", 5, false)); // "false" because we don't want to clear the context before the interpretation
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
    const auto values = get_values("a+5", false);
    const auto result = dynamic_cast<const IntegerValue*>(values.front());
    assert(result->get_actual_value() == 10);
    assert(common_ctx->get_symbol_table()->exists("a"));

    IntegerValue* value_from_table = dynamic_cast<IntegerValue*>(common_ctx->get_symbol_table()->get("a"));
    // the program returns a copy of the value set in the table
    // because it gets deallocated by the garbage collector of 'run()'
    assert(&value_from_table != &result); 

    delete result;

    print_success_msg("works with the access to a variable in maths expression", 1);
  } catch (RuntimeError e) {
    cout << e.to_string() << endl;
    assert(false);
  }
}

void test_multiline_input() {
  const auto values = get_values("5+5\n6+7\n");
  assert(values.size() == 2);
  assert(instanceof<IntegerValue>(values.front()));
  assert(instanceof<IntegerValue>(values.back()));
  const auto first = dynamic_cast<const IntegerValue*>(values.front());
  const auto second = dynamic_cast<const IntegerValue*>(values.back());
  assert(first->get_actual_value() == 10);
  assert(second->get_actual_value() == 13);

  delete first;
  delete second;

  print_success_msg("works with multiple lines", 1);
}

void test_multiline_input_and_variables() {
  // test using variables
  const auto values = get_values("store a as int = 5\na\n");
  assert(values.size() == 2);
  assert(instanceof<IntegerValue>(values.front()));
  assert(instanceof<IntegerValue>(values.back()));
  const auto first = dynamic_cast<const IntegerValue*>(values.front());
  const auto second = dynamic_cast<const IntegerValue*>(values.back());
  assert(first->get_actual_value() == 5);
  assert(second->get_actual_value() == 5);

  delete first;
  delete second;
  
  print_success_msg("works with variables on multiple lines", 1);
}

void test_variable_modification() {
  try {
    common_ctx->get_symbol_table()->clear();
    execute("store a as int = 5");
    assert(common_ctx->get_symbol_table()->exists("a"));
    assert(instanceof<IntegerValue>(common_ctx->get_symbol_table()->get("a")));

    const auto values = get_values("a = 6", false);
    assert(values.size() == 1);
    assert(instanceof<IntegerValue>(values.front()));

    const auto value = dynamic_cast<const IntegerValue*>(values.front());
    assert(value->get_actual_value() == 6);

    const auto a = dynamic_cast<IntegerValue*>(common_ctx->get_symbol_table()->get("a"));
    assert(&value != &a);
    assert(a->get_actual_value() == 6);

    delete values.front();

    print_success_msg("works with variable modification", 1);
  } catch (RuntimeError e) {
    cerr << e.to_string() << endl;
    assert(false);
  }
}

void test_variable_cloning() {
  try {
    common_ctx->get_symbol_table()->clear();
    execute("store a as int = 5");
    execute("store b as int = 10");
    execute("a = b"); // since the VarAccessNode of "b" returns a copy of the value stored in the symbol table, &a != &b
    const auto a = dynamic_cast<IntegerValue*>(common_ctx->get_symbol_table()->get("a"));
    const auto b = dynamic_cast<IntegerValue*>(common_ctx->get_symbol_table()->get("b"));
    assert(a->get_actual_value() == b->get_actual_value()); // same value
    assert(&a != &b); // but not the same memory address
    
    print_success_msg("variable cloning works", 1);
  } catch (RuntimeError e) {
    cerr << e.to_string() << endl;
    assert(false);
  }
}

void test_string() {
  assert((compare_actual_value<StringValue, string>("\"hello\"", "hello")));
  assert((compare_actual_value<StringValue, string>("'world'", "world")));
  assert((compare_actual_value<StringValue, string>("'c\\'est'", "c'est")));

  assert((compare_actual_value<StringValue, string>("'a'+'b'", "ab")));
  assert((compare_actual_value<StringValue, string>("\"hello\"+'world'", "helloworld")));
  assert((compare_actual_value<StringValue, string>("\"hello\"+' '", "hello ")));
  assert((compare_actual_value<StringValue, string>("'hello'+3", "hello3")));
  assert((compare_actual_value<StringValue, string>("3+'hello'", "3hello")));
  assert((compare_actual_value<StringValue, string>("'hello'+3.14", "hello3.14")));
  assert((compare_actual_value<StringValue, string>("'c\\'est'+' \\'ouf\\''", "c'est 'ouf'")));

  assert((compare_actual_value<StringValue, string>("'hello'*2", "hellohello")));
  assert((compare_actual_value<StringValue, string>("2*'hello'", "hellohello")));
  assert((compare_actual_value<StringValue, string>("\"hello\"*2", "hellohello")));
  assert((get_actual_value_from<StringValue, string>("\"hello\"*0")).empty());
  assert((compare_actual_value<StringValue, string>("'c\\'est'*2", "c'estc'est")));

  try {
    execute("+'hello'"); // invalid operation (a "positive string" isn't possible)
    assert(false);
  } catch (RuntimeError e) {
    assert(true); // we want the program to throw an error
  }

  try {
    execute("\"hello\"*-1"); // invalid operation
    assert(false);
  } catch (RuntimeError e) {
    assert(true); // we want the program to throw an error
  }

  try {
    execute("\"hello\"*2.45"); // invalid operation
    assert(false);
  } catch (RuntimeError e) {
    assert(true); // we want the program to throw an error
  }

  try {
    execute("\"hello\"*-3.14"); // invalid operation
    assert(false);
  } catch (RuntimeError e) {
    assert(true); // we want the program to throw an error
  }

  print_success_msg("works with strings", 1);
}

int main() {
  print_title("Interpreter tests...");

  try {
    test_empty_input();
    test_integer();
    test_double();
    test_addition_of_two_max_integers();
    test_mathematical_operation_without_parenthesis();
    test_mathematical_operation_with_parenthesis();
    test_mathematical_operation_with_negative_and_double_numbers();
    test_variable_assignment_of_an_integer();
    test_variable_assignment_of_a_double();
    test_redefinition_of_existing_variable();
    test_access_to_variable();
    test_access_to_variable_in_maths_expression();
    test_multiline_input();
    test_multiline_input_and_variables();
    test_variable_modification();
    test_variable_cloning();
    test_string();

    print_success_msg("All \"Interpreter\" tests successfully passed");
  } catch (TypeError e) {
    cerr << "ABORT. A type error was caught during interpretation:" << endl;
    cerr << e.to_string() << endl;
  } catch (RuntimeError e) {
    cerr << "ABORT. A runtime error was caught during one of the tests: " << endl;
    cerr << e.to_string() << endl;
  } catch (string cast_error) {
    cerr << "ABORT. The tests crashed due to this error :" << endl;
    cerr << cast_error << endl;
  }

  return 0;
}