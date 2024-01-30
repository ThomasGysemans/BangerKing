#include <iostream>
#include "doctest.h"
#include "../include/miscellaneous.hpp"
#include "../include/values/compositer.hpp"
#include "../include/nodes/integer_node.hpp"
#include "../include/token.hpp"
using namespace std;

DOCTEST_TEST_SUITE("miscellaneous") {
  SCENARIO("instanceof") {
    IntegerValue* integer = new IntegerValue();
    CHECK(instanceof<Value>(integer));
    CHECK(instanceof<IntegerValue>(integer));
    CHECK(!instanceof<ListValue>(integer));

    delete integer;

    unique_ptr<IntegerValue> ptr_integer = make_unique<IntegerValue>();
    CHECK(instanceof<Value>(ptr_integer));
    CHECK(instanceof<IntegerValue>(ptr_integer));
    CHECK(!instanceof<DoubleValue>(ptr_integer));

    shared_ptr<IntegerValue> shared_ptr_integer = make_shared<IntegerValue>();
    CHECK(instanceof<Value>(shared_ptr_integer));
    CHECK(instanceof<IntegerValue>(shared_ptr_integer));
    CHECK(!instanceof<DoubleValue>(shared_ptr_integer));
  }

  SCENARIO("replace substring") {
    string pattern = "world";
    string text = "hello world, what a beautiful world it is";
    replace_substring(text, pattern, "planet");
    replace_substring(text, pattern, "undefined");
    CHECK(text == "hello planet, what a beautiful planet it is");
  }

  SCENARIO("remove substring") {
    string pattern = "world";
    string text = "hello world, what a beautiful world it is";
    remove_substring(text, pattern);
    remove_substring(text, "undefined");
    CHECK(text == "hello , what a beautiful  it is");
  }

  SCENARIO("remove character") {
    char pattern = 'w';
    string text = "hello world, what a beautiful world it is";
    remove_character(text, pattern);
    remove_character(text, 'z');
    CHECK(text == "hello orld, hat a beautiful orld it is");
  }

  SCENARIO("string contains") {
    string pattern = "world";
    string text = "hello world, what a beautiful world it is";
    CHECK(string_contains(text, 'w'));
    CHECK(!string_contains(text, 'z'));
  }

  SCENARIO("cast node") {
    Token token(TokenType::NUMBER, "5", Position::getDefaultPos());
    unique_ptr<CustomNode> integer = make_unique<IntegerNode>(token);
    CHECK(integer->to_string() == "IntegerNode(5)");

    unique_ptr<IntegerNode> real_integer = cast_node<IntegerNode>(move(integer));
    CHECK(real_integer->to_string() == "IntegerNode(5)");
  }

  SCENARIO("unique cast value") {
    unique_ptr<Value> value = make_unique<IntegerValue>(67);
    CHECK(value->to_string() == "67");

    unique_ptr<IntegerValue> real_integer = cast_value<IntegerValue>(move(value));
    CHECK(real_integer->get_actual_value() == 67);
  }

  SCENARIO("shared cast value") {
    shared_ptr<Value> value = make_shared<IntegerValue>(67);
    CHECK(value->to_string() == "67");

    shared_ptr<IntegerValue> real_integer = cast_value<IntegerValue>(value);
    CHECK(real_integer->get_actual_value() == 67);
    CHECK(value != nullptr); // "value" is still accessible
    CHECK(value.get() == real_integer.get());
  }

  SCENARIO("shared cast const value") {
    shared_ptr<const Value> value = make_shared<const IntegerValue>(67);
    CHECK(value->to_string() == "67");

    shared_ptr<const IntegerValue> real_integer = cast_const_value<IntegerValue>(value);
    CHECK(real_integer->get_actual_value() == 67);
    CHECK(value != nullptr); // "value" is still accessible
    CHECK(value.get() == real_integer.get());
  }
}