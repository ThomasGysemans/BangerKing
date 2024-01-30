#include <iostream>
#include <list>
#include <limits.h>
#include "doctest.h"
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

DOCTEST_TEST_SUITE("Interpreter") {
  SCENARIO("empty input") {
    const auto values = get_values("\n\n");
    CHECK(values.empty());
  }

  SCENARIO("integer") {
    CHECK(compare_actual_value<IntegerValue>("5", 5));
    CHECK(compare_actual_value<IntegerValue>("+5", 5));
    CHECK(compare_actual_value<IntegerValue>("-5", -5));

    CHECK(compare_actual_value<IntegerValue>("5+2", 7));
    CHECK(compare_actual_value<IntegerValue>("10-5", 5));
    CHECK(compare_actual_value<IntegerValue>("2*2", 4));
    CHECK(compare_actual_value<IntegerValue>("10**2", 100));
    CHECK(compare_actual_value<IntegerValue>("10**2", 100));
    CHECK(compare_actual_value<IntegerValue>("10/3", 3));
    CHECK(compare_actual_value<IntegerValue>("10%2", 0));

    CHECK(compare_actual_value<IntegerValue>("5+-2", 3));
    CHECK(compare_actual_value<IntegerValue>("-10-5", -15));
    CHECK(compare_actual_value<IntegerValue>("-2*2", -4));
    CHECK(compare_actual_value<IntegerValue>("-10**2", 100));
    CHECK(compare_actual_value<IntegerValue>("-10/3", -3));
    CHECK(compare_actual_value<IntegerValue>("-10%2", 0));
    CHECK(compare_actual_value<IntegerValue>("10%-3", 1));
    CHECK(compare_actual_value<IntegerValue>("-10%-3", -1));
    CHECK(compare_actual_value<IntegerValue>("-10%3", -1)); // the first operand defines the sign of the result
  }

  SCENARIO("double") {
    CHECK(compare_actual_value<DoubleValue>("3.14", 3.14));
    CHECK(compare_actual_value<DoubleValue>("+3.14", 3.14));
    CHECK(compare_actual_value<DoubleValue>("-3.14", -3.14));

    // // Additions
    CHECK(compare_actual_value<DoubleValue>("1+0.2", 1.2));
    CHECK(compare_actual_value<DoubleValue>("0.1+1", 1.1));
    CHECK((get_actual_value_from<DoubleValue, double>("0.1+0.2")) != 0.3); // when using doubles, 0.1+0.2 != 0.3
    CHECK(get_custom_value_from<DoubleValue>("0.1+0.2")->to_string() == "0.3"); // however it should still print the right value
    CHECK(compare_actual_value<DoubleValue>("5.0+-1", 4.0));
    // Substraction
    CHECK(compare_actual_value<DoubleValue>("1-0.5", 0.5));
    CHECK(compare_actual_value<DoubleValue>("1.5-0.5", 1.0));
    CHECK(compare_actual_value<DoubleValue>("0.5-0.5", 0.0));
    CHECK(compare_actual_value<DoubleValue>("0.5- -0.5", 1.0)); // a whitespace is needed because '--' is the decrement operator
    // Multiplications
    CHECK(compare_actual_value<DoubleValue>("2.2*2", 4.4));
    CHECK(compare_actual_value<DoubleValue>("2*2.5", 5.0));
    CHECK(compare_actual_value<DoubleValue>("2.5*0.1", 0.25));
    CHECK(compare_actual_value<DoubleValue>("2.5*-0.1", -0.25));
    // Power
    CHECK(compare_actual_value<DoubleValue>("10**2.0", 100.0));
    CHECK(compare_actual_value<DoubleValue>("10.0**2", 100.0));
    CHECK(compare_actual_value<DoubleValue>("10.0**2.0", 100.0));
    CHECK(compare_actual_value<DoubleValue>("10.0**-2.0", 0.01));
    // Divisions
    CHECK(compare_actual_value<DoubleValue>("10/2.0", 5.0));
    CHECK(compare_actual_value<DoubleValue>("2.0/2", 1.0));
    CHECK(compare_actual_value<DoubleValue>("5.0/2.0", 2.5));
    CHECK(compare_actual_value<DoubleValue>("5.0/-2.0", -2.5));
    // Modulo
    CHECK(compare_actual_value<DoubleValue>("10%2.0", 0.0));
    CHECK(compare_actual_value<DoubleValue>("2.0%2", 0.0));
    CHECK(compare_actual_value<DoubleValue>("5.0%2.0", 1.0));
    CHECK(compare_actual_value<DoubleValue>("5.0%-2.0", 1));
    CHECK(compare_actual_value<DoubleValue>("-5.0%-2.0", -1));
    CHECK(compare_actual_value<DoubleValue>("-5.0%2.0", -1)); // the first operand defines the sign of the result
  }

  SCENARIO("addition of two max integers") {
    const string half = std::to_string(INT_MAX / 2);
    CHECK(compare_actual_value<IntegerValue>(half + " + " + half, (INT_MAX - 1))); // (-1 because INT_MAX is not even, and so is rounded down)
  }

  SCENARIO("mathematical operation without parenthesis") {
    CHECK(compare_actual_value<IntegerValue>("5 * 2 -2 / 2", 9));
  }

  SCENARIO("mathematical operation with parenthesis") {
    CHECK(compare_actual_value<IntegerValue>("5 * (2 - 2) / 2", 0));
  }

  SCENARIO("mathematical operation with negative and double numbers") {
    CHECK(compare_actual_value<DoubleValue>("(-2.5 * 2 / -5.0) * 10 ** 2", 100));
  }

  SCENARIO("variable assignment of an integer") {
    CHECK(compare_actual_value<IntegerValue>("store a as int = 5", 5));
    CHECK(common_ctx->get_symbol_table()->exists("a"));
    CHECK(instanceof<IntegerValue>(common_ctx->get_symbol_table()->get("a").get()));

    CHECK(compare_actual_value<IntegerValue>("store b as int", IntegerValue::get_default_value()));
    CHECK(common_ctx->get_symbol_table()->exists("b"));
    CHECK(instanceof<IntegerValue>(common_ctx->get_symbol_table()->get("b").get()));

    CHECK(compare_actual_value<IntegerValue>("store c as int = 5.6", 5));
    CHECK(common_ctx->get_symbol_table()->exists("c"));
    CHECK(instanceof<IntegerValue>(common_ctx->get_symbol_table()->get("c").get()));
  }

  SCENARIO("variable assignment of a double") {
    CHECK(compare_actual_value<DoubleValue>("store a as double = 5", 5));
    CHECK(common_ctx->get_symbol_table()->exists("a"));
    CHECK(instanceof<DoubleValue>(common_ctx->get_symbol_table()->get("a").get()));

    CHECK(compare_actual_value<DoubleValue>("store b as double = 5.5", 5.5));
    CHECK(common_ctx->get_symbol_table()->exists("b"));
    CHECK(instanceof<DoubleValue>(common_ctx->get_symbol_table()->get("b").get()));

    CHECK(compare_actual_value<DoubleValue>("store c as double", DoubleValue::get_default_value()));
    CHECK(common_ctx->get_symbol_table()->exists("c"));
    CHECK(instanceof<DoubleValue>(common_ctx->get_symbol_table()->get("c").get()));
  }

  SCENARIO("variable assignment with unknown type") {
    common_ctx->get_symbol_table()->clear();
    CHECK_THROWS_AS(execute("store a as zzz"), TypeError);
    CHECK(!common_ctx->get_symbol_table()->exists("a"));
  }

  SCENARIO("redefinition of existing variable") {
    common_ctx->get_symbol_table()->clear();
    CHECK_NOTHROW(execute("store a as int = 4"));
    CHECK_THROWS_AS(execute("store a as int = 1"), RuntimeError);
  }

  SCENARIO("access to variable") {
    common_ctx->get_symbol_table()->clear();
    CHECK_NOTHROW(execute("store a as int = 5"));
    CHECK(compare_actual_value<IntegerValue>("a", 5, false)); // "false" because we don't want to clear the context before the interpretation
    CHECK(common_ctx->get_symbol_table()->exists("a"));
  }

  SCENARIO("access to variable in maths expression") {
    common_ctx->get_symbol_table()->clear();
    CHECK_NOTHROW(execute("store a as int = 5"));
    CHECK(common_ctx->get_symbol_table()->exists("a"));

    const auto values = get_values("a+5", false);
    shared_ptr<const Value> front = values.front();
    shared_ptr<const IntegerValue> result = cast_const_value<IntegerValue>(front);
    CHECK(result->get_actual_value() == 10);

    unique_ptr<IntegerValue> value_from_table = cast_value<IntegerValue>(common_ctx->get_symbol_table()->get("a"));

    // "get()" returns a copy form the actual value stored in the symbol table
    CHECK(value_from_table.get() != result.get()); 
    CHECK(value_from_table->get_actual_value() == 5);
  }

  SCENARIO("multiline input") {
    const auto values = get_values("5+5\n6+7\n");
    CHECK(values.size() == 2);
    shared_ptr<const Value> front = values.front();
    shared_ptr<const Value> back = values.back();
    shared_ptr<const IntegerValue> first = cast_const_value<IntegerValue>(front);
    shared_ptr<const IntegerValue> second = cast_const_value<IntegerValue>(back);
    CHECK(first->get_actual_value() == 10);
    CHECK(second->get_actual_value() == 13);
  }

  SCENARIO("multiline input and variables") {
    // test using variables
    const auto values = get_values("store a as int = 5\na\n");
    CHECK(values.size() == 2);
    auto front = values.front();
    auto back = values.back();
    auto first = cast_const_value<IntegerValue>(front);
    auto second = cast_const_value<IntegerValue>(back);
    CHECK(first->get_actual_value() == 5);
    CHECK(second->get_actual_value() == 5);
  }

  SCENARIO("variable modification") {
    common_ctx->get_symbol_table()->clear();
    CHECK_NOTHROW(execute("store a as int = 5"));
    CHECK(common_ctx->get_symbol_table()->exists("a"));
    CHECK(instanceof<IntegerValue>(common_ctx->get_symbol_table()->get("a").get()));

    const auto values = get_values("a = 6", false);
    CHECK(values.size() == 1);
    shared_ptr<const Value> front = values.front();
    CHECK(instanceof<IntegerValue>(front));

    shared_ptr<const IntegerValue> value = cast_const_value<IntegerValue>(front);
    CHECK(value->get_actual_value() == 6);

    unique_ptr<IntegerValue> a = cast_value<IntegerValue>(common_ctx->get_symbol_table()->get("a"));
    CHECK(value.get() != a.get());
    CHECK(a->get_actual_value() == 6);
  }

  SCENARIO("variables not sharing same memory address") {
    common_ctx->get_symbol_table()->clear();
    CHECK_NOTHROW(execute("store a as int = 5"));
    CHECK_NOTHROW(execute("store b as int = 10"));
    CHECK_NOTHROW(execute("a = b")); // since the VarAccessNode of "b" returns a copy of the value stored in the symbol table, &a != &b
    auto a = cast_value<IntegerValue>(common_ctx->get_symbol_table()->get("a"));
    auto b = cast_value<IntegerValue>(common_ctx->get_symbol_table()->get("b"));
    CHECK(a->get_actual_value() == b->get_actual_value()); // same value
    CHECK(a.get() != b.get()); // but not the same memory address
  }

  SCENARIO("variables type error") {
    common_ctx->get_symbol_table()->clear();
    CHECK_THROWS_AS(execute("store a as double = 'hello'"), TypeError);
  }

  SCENARIO("string") {
    CHECK((compare_actual_value<StringValue, string>("\"hello\"", "hello")));
    CHECK((compare_actual_value<StringValue, string>("'world'", "world")));
    CHECK((compare_actual_value<StringValue, string>("'c\\'est'", "c'est")));

    CHECK((compare_actual_value<StringValue, string>("'a'+'b'", "ab")));
    CHECK((compare_actual_value<StringValue, string>("\"hello\"+'world'", "helloworld")));
    CHECK((compare_actual_value<StringValue, string>("\"hello\"+' '", "hello ")));
    CHECK((compare_actual_value<StringValue, string>("'hello'+3", "hello3")));
    CHECK((compare_actual_value<StringValue, string>("3+'hello'", "3hello")));
    CHECK((compare_actual_value<StringValue, string>("'hello'+3.14", "hello3.14")));
    CHECK((compare_actual_value<StringValue, string>("'c\\'est'+' \\'ouf\\''", "c'est 'ouf'")));

    CHECK((compare_actual_value<StringValue, string>("'hello'*2", "hellohello")));
    CHECK((compare_actual_value<StringValue, string>("2*'hello'", "hellohello")));
    CHECK((compare_actual_value<StringValue, string>("\"hello\"*2", "hellohello")));
    CHECK((get_actual_value_from<StringValue, string>("\"hello\"*0")).empty());
    CHECK((compare_actual_value<StringValue, string>("'c\\'est'*2", "c'estc'est")));

    CHECK((compare_actual_value<StringValue, string>("store a as string", StringValue::get_default_value())));
    CHECK(common_ctx->get_symbol_table()->exists("a"));
    auto a = cast_value<StringValue>(common_ctx->get_symbol_table()->get("a"));
    CHECK(a->get_actual_value() == StringValue::get_default_value());

    CHECK_THROWS_AS(execute("+'hello'"), RuntimeError); // invalid operation (a "positive string" isn't possible)
    CHECK_THROWS_AS(execute("\"hello\"*-1"), RuntimeError); // invalid operation
    CHECK_THROWS_AS(execute("\"hello\"*2.45"), RuntimeError); // invalid operation
    CHECK_THROWS_AS(execute("\"hello\"*-3.14"), RuntimeError); // invalid operation
  }

  SCENARIO("unclosed string") {
    CHECK_THROWS_AS(execute("'not closed simple quote"), UnclosedStringError);
    CHECK_THROWS_AS(execute("\"not closed simple quote"), UnclosedStringError);
    CHECK_THROWS_AS(execute("\"not closed simple quote'"), UnclosedStringError);
    CHECK_THROWS_AS(execute("'not closed simple quote\\'"), UnclosedStringError);
  }

  SCENARIO("multiline string") {
    CHECK((compare_actual_value<StringValue, string>("\"hello\nworld\"", "hello\nworld")));
    CHECK((compare_actual_value<StringValue, string>("'hello\nworld'", "hello\nworld")));
  }

  SCENARIO("boolean") {
    CHECK((compare_actual_value<BooleanValue, bool>("true", true)));
    CHECK((compare_actual_value<BooleanValue, bool>("false", false)));
    
    CHECK((compare_actual_value<IntegerValue, int>("true+true", (true+true))));
    CHECK((compare_actual_value<IntegerValue, int>("true+false", (true+false))));
    CHECK((compare_actual_value<IntegerValue, int>("false+true", (false+true))));
    CHECK((compare_actual_value<IntegerValue, int>("false+false", (false+false))));

    CHECK((compare_actual_value<IntegerValue, int>("true-false", (true-false))));
    CHECK((compare_actual_value<IntegerValue, int>("true*2", 2)));
    CHECK((compare_actual_value<IntegerValue, int>("true/2", 0))); // 1 / 2 = 0.5, but as an integer it's 0
    CHECK((compare_actual_value<IntegerValue, int>("true%2", 1)));
    CHECK((compare_actual_value<IntegerValue, int>("true**2", 1)));
  }

  SCENARIO("division by zero") {
    // DIVISION 

    CHECK_THROWS_AS(execute("5/0"), ArithmeticError);
    CHECK_THROWS_AS(execute("5/0.0"), ArithmeticError);
    CHECK_THROWS_AS(execute("5.0/0"), ArithmeticError);
    CHECK_THROWS_AS(execute("5.0/0.0"), ArithmeticError);

    // MODULO

    CHECK_THROWS_AS(execute("5%0"), ArithmeticError);
    CHECK_THROWS_AS(execute("5%0.0"), ArithmeticError);
    CHECK_THROWS_AS(execute("5.0%0"), ArithmeticError);
    CHECK_THROWS_AS(execute("5.0%0.0"), ArithmeticError);
  }

  SCENARIO("max integer in variable") {
    common_ctx->get_symbol_table()->clear();
    
    CHECK_NOTHROW(execute("store a as int = " + std::to_string(INT_MAX)));
    CHECK(common_ctx->get_symbol_table()->exists("a"));
    CHECK(cast_value<IntegerValue>(common_ctx->get_symbol_table()->get("a"))->get_actual_value() == INT_MAX);

    CHECK_NOTHROW(execute("store b as double = " + std::to_string(numeric_limits<double>::max())));
    CHECK(common_ctx->get_symbol_table()->exists("b"));
    CHECK(cast_value<DoubleValue>(common_ctx->get_symbol_table()->get("b"))->get_actual_value() == numeric_limits<double>::max());
    
    const string code = "store c as int = " + std::to_string(LONG_MAX);
    CHECK_THROWS_AS(execute(code), TypeOverflowError);
    CHECK(!common_ctx->get_symbol_table()->exists("c"));

    const string code2 = "store d as double = 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368555555555.000000";
    CHECK_THROWS_AS(execute(code2), TypeOverflowError);
    CHECK(!common_ctx->get_symbol_table()->exists("d"));
  }

  SCENARIO("constants") {
    CHECK(compare_actual_value<IntegerValue>("define a as int = 5", 5));
    CHECK(common_ctx->get_symbol_table()->exists("a"));
    CHECK(common_ctx->get_symbol_table()->is_constant("a"));
    CHECK(instanceof<IntegerValue>(common_ctx->get_symbol_table()->get("a").get()));

    CHECK(compare_actual_value<IntegerValue>("define b as int = 5.5", 5));
    CHECK(common_ctx->get_symbol_table()->exists("b"));
    CHECK(common_ctx->get_symbol_table()->is_constant("b"));
    CHECK(instanceof<IntegerValue>(common_ctx->get_symbol_table()->get("b").get()));

    CHECK_THROWS_AS(execute("b = 8"), TypeError);
  }

  SCENARIO("simple boolean operators AND OR NOT") {
    CHECK(compare_actual_value<BooleanValue>("true and true", true));
    CHECK(compare_actual_value<BooleanValue>("true and false", false));
    CHECK(compare_actual_value<BooleanValue>("false and true", false));
    CHECK(compare_actual_value<BooleanValue>("false and false", false));
    CHECK(compare_actual_value<BooleanValue>("false and (false)", false));
    CHECK(compare_actual_value<BooleanValue>("(false) and (false)", false));
    CHECK(compare_actual_value<BooleanValue>("((false) and (false))", false));

    CHECK(compare_actual_value<BooleanValue>("true or true", true));
    CHECK(compare_actual_value<BooleanValue>("true or false", true));
    CHECK(compare_actual_value<BooleanValue>("false or true", true));
    CHECK(compare_actual_value<BooleanValue>("false or false", false));

    // OR returns the truthy expression
    // instead of a simple boolean value
    CHECK(compare_actual_value<IntegerValue>("0 or 5", 5));

    // In the case of an AND operation,
    // the right operand is ignored
    // if the left one is false
    CHECK(compare_actual_value<BooleanValue>("0 and (store a as int = 5)", false));
    CHECK(!common_ctx->get_symbol_table()->exists("a"));

    CHECK(compare_actual_value<BooleanValue>("not true", false));
    CHECK(compare_actual_value<BooleanValue>("not false", true));
    CHECK(compare_actual_value<BooleanValue>("! true", false));
    CHECK(compare_actual_value<BooleanValue>("! false", true));
    CHECK(compare_actual_value<BooleanValue>("!true", false));
    CHECK(compare_actual_value<BooleanValue>("!false", true));
    CHECK(compare_actual_value<BooleanValue>("!(true)", false));
    CHECK(compare_actual_value<BooleanValue>("!(false)", true));
    CHECK(compare_actual_value<BooleanValue>("!!(false)", false));
  }

  SCENARIO("or node with variable assignments") {
    // In this code,
    // we assign a copy of the variable "b"
    // to the variable "a", created after "b",
    // but "c" should not get created, because "b" is truthy.
    // The return value should be 2.
    const string code = "store a as int = (store b as int = 2) or (store c as int = 3)";
    CHECK(compare_actual_value<IntegerValue>(code, 2));
  }
}