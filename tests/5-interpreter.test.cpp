#include <iostream>
#include <list>
#include <limits.h>
#include "helper.hpp"
#include "../include/lexer.hpp"
#include "../include/token.hpp"
#include "../include/parser.hpp"
#include "../include/interpreter.hpp"
#include "../include/symbol_table.hpp"
#include "../include/values/compositer.hpp"
#include "../include/exceptions/runtime_error.hpp"
#include "../include/exceptions/arithmetic_error.hpp"
#include "../include/exceptions/type_error.hpp"
#include "../include/exceptions/unclosed_string_error.hpp"
#include "../include/exceptions/type_overflow_error.hpp"
#include "../include/types.hpp"
using namespace std;

shared_ptr<Context> common_ctx = make_shared<Context>("<tests>");

/// @brief Gets the values that the interpreter returns from the given code. Use this for the tests.
/// @param code The code to interpret.
/// @param clear_ctx Whether or not the global context should be cleared before interpreting the given source code. Defaults to `true`.
/// @return The values that the interpreter calculated.
list<shared_ptr<const Value>> get_values(const string& code, bool clear_ctx = true) {
  if (clear_ctx) common_ctx->get_symbol_table()->clear();

  Lexer lexer(&code);
  READ_FILES.insert({ "<stdin>", make_unique<string>(code) });
  list<unique_ptr<const Token>> tokens = lexer.generate_tokens();
  Parser parser(tokens);
  unique_ptr<ListNode> tree = parser.parse();

  Interpreter::set_shared_ctx(common_ctx);
  unique_ptr<RuntimeResult> result = Interpreter::visit(move(tree));
  shared_ptr<Value> v = result->get_value();
  if (v == nullptr) {
    throw string("Segmentation fault happened during interpretation of this code : " + code + " because the result is a `nullptr`.");
  }
  shared_ptr<ListValue> values = cast_value<ListValue>(v);
  if (values == nullptr) {
    throw string("Invalid cast of interpreter result. It did not return a valid instance of ListValue.");
  }
  return values->get_elements();
}

void execute(const string& code) {
  Lexer lexer(&code);
  list<unique_ptr<const Token>> tokens = lexer.generate_tokens();
  Parser parser(tokens);
  unique_ptr<ListNode> tree = parser.parse();
  
  Interpreter::set_shared_ctx(common_ctx);
  Interpreter::visit(move(tree));
}

template <typename V>
shared_ptr<const V> get_custom_value_from(const string& code, bool clear_ctx = true) {
  return cast_const_value<V>(get_values(code, clear_ctx).front());
}

template <typename V, typename N>
N get_actual_value_from(const string& code, bool clear_ctx = true) {
  return get_custom_value_from<V>(code, clear_ctx)->get_actual_value();
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

  // // Additions
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
  assert(instanceof<IntegerValue>(common_ctx->get_symbol_table()->get("a").get()));

  assert(compare_actual_value<IntegerValue>("store b as int", IntegerValue::get_default_value()));
  assert(common_ctx->get_symbol_table()->exists("b"));
  assert(instanceof<IntegerValue>(common_ctx->get_symbol_table()->get("b").get()));

  assert(compare_actual_value<IntegerValue>("store c as int = 5.6", 5));
  assert(common_ctx->get_symbol_table()->exists("c"));
  assert(instanceof<IntegerValue>(common_ctx->get_symbol_table()->get("c").get()));

  print_success_msg("works with the variable assignment of an integer", 1);
}

void test_variable_assignment_of_a_double() {
  assert(compare_actual_value<DoubleValue>("store a as double = 5", 5));
  assert(common_ctx->get_symbol_table()->exists("a"));
  assert(instanceof<DoubleValue>(common_ctx->get_symbol_table()->get("a").get()));

  assert(compare_actual_value<DoubleValue>("store b as double = 5.5", 5.5));
  assert(common_ctx->get_symbol_table()->exists("b"));
  assert(instanceof<DoubleValue>(common_ctx->get_symbol_table()->get("b").get()));

  assert(compare_actual_value<DoubleValue>("store c as double", DoubleValue::get_default_value()));
  assert(common_ctx->get_symbol_table()->exists("c"));
  assert(instanceof<DoubleValue>(common_ctx->get_symbol_table()->get("c").get()));

  print_success_msg("works with the variable assignment of a double", 1);
}

void test_variable_assignment_with_unknown_type() {
  common_ctx->get_symbol_table()->clear();

  try {
    execute("store a as zzz");
    assert(false);
  } catch (TypeError e) {
    assert(!common_ctx->get_symbol_table()->exists("a"));
  }

  print_success_msg("throws an error if the type of a variable is unknown", 1);
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
    
    print_success_msg("works with an access to a variable", 1);
  } catch (RuntimeError e) {
    cout << e.to_string() << endl;
    assert(false);
  }
}

void test_access_to_variable_in_maths_expression() {
  try {
    common_ctx->get_symbol_table()->clear();
    execute("store a as int = 5");
    assert(common_ctx->get_symbol_table()->exists("a"));

    const auto values = get_values("a+5", false);
    shared_ptr<const Value> front = values.front();
    shared_ptr<const IntegerValue> result = cast_const_value<IntegerValue>(front);
    assert(result->get_actual_value() == 10);

    unique_ptr<IntegerValue> value_from_table = cast_value<IntegerValue>(common_ctx->get_symbol_table()->get("a"));

    // "get()" returns a copy form the actual value stored in the symbol table
    assert(value_from_table.get() != result.get()); 
    assert(value_from_table->get_actual_value() == 5);

    print_success_msg("works with the access to a variable in maths expression", 1);
  } catch (RuntimeError e) {
    cout << e.to_string() << endl;
    assert(false);
  }
}

void test_multiline_input() {
  const auto values = get_values("5+5\n6+7\n");
  assert(values.size() == 2);
  shared_ptr<const Value> front = values.front();
  shared_ptr<const Value> back = values.back();
  shared_ptr<const IntegerValue> first = cast_const_value<IntegerValue>(front);
  shared_ptr<const IntegerValue> second = cast_const_value<IntegerValue>(back);
  assert(first->get_actual_value() == 10);
  assert(second->get_actual_value() == 13);

  print_success_msg("works with multiple lines", 1);
}

void test_multiline_input_and_variables() {
  // test using variables
  const auto values = get_values("store a as int = 5\na\n");
  assert(values.size() == 2);
  auto front = values.front();
  auto back = values.back();
  auto first = cast_const_value<IntegerValue>(front);
  auto second = cast_const_value<IntegerValue>(back);
  assert(first->get_actual_value() == 5);
  assert(second->get_actual_value() == 5);

  print_success_msg("works with variables on multiple lines", 1);
}

void test_variable_modification() {
  try {
    common_ctx->get_symbol_table()->clear();
    execute("store a as int = 5");
    assert(common_ctx->get_symbol_table()->exists("a"));
    assert(instanceof<IntegerValue>(common_ctx->get_symbol_table()->get("a").get()));

    const auto values = get_values("a = 6", false);
    assert(values.size() == 1);
    shared_ptr<const Value> front = values.front();
    assert(instanceof<IntegerValue>(front));

    shared_ptr<const IntegerValue> value = cast_const_value<IntegerValue>(front);
    assert(value->get_actual_value() == 6);

    unique_ptr<IntegerValue> a = cast_value<IntegerValue>(common_ctx->get_symbol_table()->get("a"));
    assert(value.get() != a.get());
    assert(a->get_actual_value() == 6);

    print_success_msg("works with variable modification", 1);
  } catch (RuntimeError e) {
    cerr << e.to_string() << endl;
    assert(false);
  }
}

void test_variables_not_sharing_same_memory_address() {
  try {
    common_ctx->get_symbol_table()->clear();
    execute("store a as int = 5");
    execute("store b as int = 10");
    execute("a = b"); // since the VarAccessNode of "b" returns a copy of the value stored in the symbol table, &a != &b
    auto a = cast_value<IntegerValue>(common_ctx->get_symbol_table()->get("a"));
    auto b = cast_value<IntegerValue>(common_ctx->get_symbol_table()->get("b"));
    assert(a->get_actual_value() == b->get_actual_value()); // same value
    assert(a.get() != b.get()); // but not the same memory address
    
    print_success_msg("variable cloning works", 1);
  } catch (RuntimeError e) {
    cerr << e.to_string() << endl;
    assert(false);
  }
}

void test_variables_type_error() {
  try {
    common_ctx->get_symbol_table()->clear();
    execute("store a as double = 'hello'");
    assert(false);
  } catch (TypeError e) {
    assert(true);
    print_success_msg("throws type error if type isn't castable", 1);
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

  assert((compare_actual_value<StringValue, string>("store a as string", StringValue::get_default_value())));
  assert(common_ctx->get_symbol_table()->exists("a"));
  auto a = cast_value<StringValue>(common_ctx->get_symbol_table()->get("a"));
  assert(a->get_actual_value() == StringValue::get_default_value());

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

void test_unclosed_string() {
  try {
    execute("'not closed simple quote");
    assert(false);
  } catch (UnclosedStringError e) {
    assert(true);
  }

  try {
    execute("\"not closed simple quote");
    assert(false);
  } catch (UnclosedStringError e) {
    assert(true);
  }

  try {
    execute("\"not closed simple quote'");
    assert(false);
  } catch (UnclosedStringError e) {
    assert(true);
  }

  try {
    execute("'not closed simple quote\\'");
    assert(false);
  } catch (UnclosedStringError e) {
    assert(true);
  }
  
  print_success_msg("Unclosed string are detected", 1);
}

void test_multiline_string() {
  assert((compare_actual_value<StringValue, string>("\"hello\nworld\"", "hello\nworld")));
  assert((compare_actual_value<StringValue, string>("'hello\nworld'", "hello\nworld")));

  print_success_msg("multiline strings work", 1);
}

void test_boolean() {
  assert((compare_actual_value<BooleanValue, bool>("true", true)));
  assert((compare_actual_value<BooleanValue, bool>("false", false)));
  
  assert((compare_actual_value<IntegerValue, int>("true+true", (true+true))));
  assert((compare_actual_value<IntegerValue, int>("true+false", (true+false))));
  assert((compare_actual_value<IntegerValue, int>("false+true", (false+true))));
  assert((compare_actual_value<IntegerValue, int>("false+false", (false+false))));

  assert((compare_actual_value<IntegerValue, int>("true-false", (true-false))));
  assert((compare_actual_value<IntegerValue, int>("true*2", 2)));
  assert((compare_actual_value<IntegerValue, int>("true/2", 0))); // 1 / 2 = 0.5, but as an integer it's 0
  assert((compare_actual_value<IntegerValue, int>("true%2", 1)));
  assert((compare_actual_value<IntegerValue, int>("true**2", 1)));

  print_success_msg("works with booleans true and false and operations involving booleans", 1);
}

void test_division_by_zero() {
  // DIVISION 

  try {
    execute("5/0");
    assert(false);
  } catch (ArithmeticError e) {}

  try {
    execute("5/0.0");
    assert(false);
  } catch (ArithmeticError e) {}

  try {
    execute("5.0/0");
    assert(false);
  } catch (ArithmeticError e) {}

  try {
    execute("5.0/0.0");
    assert(false);
  } catch (ArithmeticError e) {}

  // MODULO

  try {
    execute("5%0");
    assert(false);
  } catch (ArithmeticError e) {}

  try {
    execute("5%0.0");
    assert(false);
  } catch (ArithmeticError e) {}

  try {
    execute("5.0%0");
    assert(false);
  } catch (ArithmeticError e) {}

  try {
    execute("5.0%0.0");
    assert(false);
  } catch (ArithmeticError e) {}

  print_success_msg("division by zero isn't possible", 1);
}

void test_max_integer_in_variable() {
  common_ctx->get_symbol_table()->clear();
  
  try {
    execute("store a as int = " + std::to_string(INT_MAX));
    assert(common_ctx->get_symbol_table()->exists("a"));
    assert(cast_value<IntegerValue>(common_ctx->get_symbol_table()->get("a"))->get_actual_value() == INT_MAX);
  } catch (CustomError e) {
    assert(false);
  }

  try {
    execute("store b as double = " + std::to_string(numeric_limits<double>::max()));
    assert(common_ctx->get_symbol_table()->exists("b"));
    assert(cast_value<DoubleValue>(common_ctx->get_symbol_table()->get("b"))->get_actual_value() == numeric_limits<double>::max());
  } catch (CustomError e) {
    assert(false);
  }
  
  try {
    const string code = "store c as int = " + std::to_string(LONG_MAX);
    execute(code);
    assert(false);
  } catch (TypeOverflowError e) {
    assert(!common_ctx->get_symbol_table()->exists("c"));
  }

  try {
    const string code = "store d as double = 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368555555555.000000";
    execute(code);
    assert(false);
  } catch (TypeOverflowError e) {
    assert(!common_ctx->get_symbol_table()->exists("d"));
  }

  print_success_msg("stores MAX_INT and max double in variables and throws error if exceeding", 1);
}

void test_constants() {
  assert(compare_actual_value<IntegerValue>("define a as int = 5", 5));
  assert(common_ctx->get_symbol_table()->exists("a"));
  assert(common_ctx->get_symbol_table()->is_constant("a"));
  assert(instanceof<IntegerValue>(common_ctx->get_symbol_table()->get("a").get()));

  assert(compare_actual_value<IntegerValue>("define b as int = 5.5", 5));
  assert(common_ctx->get_symbol_table()->exists("b"));
  assert(common_ctx->get_symbol_table()->is_constant("b"));
  assert(instanceof<IntegerValue>(common_ctx->get_symbol_table()->get("b").get()));

  try {
    execute("b = 8");
    assert(false);
  } catch (TypeError e) { }

  print_success_msg("stores constants and it cannot get modified", 1);
}

void test_simple_boolean_operators_AND_OR_NOT() {
  assert(compare_actual_value<BooleanValue>("true and true", true));
  assert(compare_actual_value<BooleanValue>("true and false", false));
  assert(compare_actual_value<BooleanValue>("false and true", false));
  assert(compare_actual_value<BooleanValue>("false and false", false));
  assert(compare_actual_value<BooleanValue>("false and (false)", false));
  assert(compare_actual_value<BooleanValue>("(false) and (false)", false));
  assert(compare_actual_value<BooleanValue>("((false) and (false))", false));

  assert(compare_actual_value<BooleanValue>("true or true", true));
  assert(compare_actual_value<BooleanValue>("true or false", true));
  assert(compare_actual_value<BooleanValue>("false or true", true));
  assert(compare_actual_value<BooleanValue>("false or false", false));

  // OR returns the truthy expression
  // instead of a simple boolean value
  assert(compare_actual_value<IntegerValue>("0 or 5", 5));

  // In the case of an AND operation,
  // the right operand is ignored
  // if the left one is false
  assert(compare_actual_value<BooleanValue>("0 and (store a as int = 5)", false));
  assert(!common_ctx->get_symbol_table()->exists("a"));

  assert(compare_actual_value<BooleanValue>("not true", false));
  assert(compare_actual_value<BooleanValue>("not false", true));
  assert(compare_actual_value<BooleanValue>("! true", false));
  assert(compare_actual_value<BooleanValue>("! false", true));
  assert(compare_actual_value<BooleanValue>("!true", false));
  assert(compare_actual_value<BooleanValue>("!false", true));
  assert(compare_actual_value<BooleanValue>("!(true)", false));
  assert(compare_actual_value<BooleanValue>("!(false)", true));
  assert(compare_actual_value<BooleanValue>("!!(false)", false));

  print_success_msg("works with simple boolean operators (and, or, not)", 1);
}

void test_or_node_with_variable_assignments() {
  // In this code,
  // we assign a copy of the variable "b"
  // to the variable "a", created after "b",
  // but "c" should not get created, because "b" is truthy.
  // The return value should be 2.
  const string code = "store a as int = (store b as int = 2) or (store c as int = 3)";
  assert(compare_actual_value<IntegerValue>(code, 2));

  print_success_msg("works with variable assignments in OR node", 1);
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
    test_variable_assignment_with_unknown_type();
    test_redefinition_of_existing_variable();
    test_access_to_variable();
    test_access_to_variable_in_maths_expression();
    test_multiline_input();
    test_multiline_input_and_variables();
    test_variable_modification();
    test_variables_not_sharing_same_memory_address();
    test_variables_type_error();
    test_string();
    test_unclosed_string();
    test_multiline_string();
    test_boolean();
    test_division_by_zero();
    test_max_integer_in_variable();
    test_constants();
    test_simple_boolean_operators_AND_OR_NOT();
    test_or_node_with_variable_assignments();

    print_success_msg("All \"Interpreter\" tests successfully passed");
  } catch (TypeError e) {
    cerr << "ABORT. A type error was caught during interpretation:" << endl;
    cerr << e.to_string() << endl;
  } catch (RuntimeError e) {
    cerr << "ABORT. A runtime error was caught during one of the tests: " << endl;
    cerr << e.to_string() << endl;
  } catch (InvalidSyntaxError e) {
    cerr << "ABORT. A lexer/parser error was caught during one of the tests: " << endl;
    cerr << e.to_string() << endl;
  } catch (string cast_error) {
    cerr << "ABORT. The tests crashed due to this error :" << endl;
    cerr << cast_error << endl;
  }

  return 0;
}