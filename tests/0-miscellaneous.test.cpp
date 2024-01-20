#include <iostream>
#include "helper.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/values/compositer.hpp"
#include "../include/nodes/integer_node.hpp"
#include "../include/token.hpp"
using namespace std;

void test_instanceof() {
  IntegerValue* integer = new IntegerValue();
  assert(instanceof<Value>(integer));
  assert(instanceof<IntegerValue>(integer));
  assert(!instanceof<ListValue>(integer));

  delete integer;

  unique_ptr<IntegerValue> ptr_integer = make_unique<IntegerValue>();
  assert(instanceof<Value>(ptr_integer));
  assert(instanceof<IntegerValue>(ptr_integer));
  assert(!instanceof<DoubleValue>(ptr_integer));

  shared_ptr<IntegerValue> shared_ptr_integer = make_shared<IntegerValue>();
  assert(instanceof<Value>(shared_ptr_integer));
  assert(instanceof<IntegerValue>(shared_ptr_integer));
  assert(!instanceof<DoubleValue>(shared_ptr_integer));

  print_success_msg("instanceof", 1);
}

void test_replace_substring() {
  string pattern = "world";
  string text = "hello world, what a beautiful world it is";
  replace_substring(text, pattern, "planet");
  replace_substring(text, pattern, "undefined");
  assert(text == "hello planet, what a beautiful planet it is");

  print_success_msg("replace_substring", 1);
}

void test_remove_substring() {
  string pattern = "world";
  string text = "hello world, what a beautiful world it is";
  remove_substring(text, pattern);
  remove_substring(text, "undefined");
  assert(text == "hello , what a beautiful  it is");

  print_success_msg("remove_substring", 1);
}

void test_remove_character() {
  char pattern = 'w';
  string text = "hello world, what a beautiful world it is";
  remove_character(text, pattern);
  remove_character(text, 'z');
  assert(text == "hello orld, hat a beautiful orld it is");

  print_success_msg("remove_character", 1);
}

void test_string_contains() {
  string pattern = "world";
  string text = "hello world, what a beautiful world it is";
  assert(string_contains(text, 'w'));
  assert(!string_contains(text, 'z'));

  print_success_msg("string_contains", 1);
}

void test_cast_node() {
  Token token(TokenType::NUMBER, "5", Position::getDefaultPos());
  unique_ptr<CustomNode> integer = make_unique<IntegerNode>(token);
  assert(integer->to_string() == "IntegerNode(5)");

  unique_ptr<IntegerNode> real_integer = cast_node<IntegerNode>(move(integer));
  assert(real_integer->getValue() == 5);
  assert(real_integer->to_string() == "IntegerNode(5)");

  print_success_msg("cast_node", 1);
}

void test_unique_cast_value() {
  unique_ptr<Value> value = make_unique<IntegerValue>(67);
  assert(value->to_string() == "67");

  unique_ptr<IntegerValue> real_integer = cast_value<IntegerValue>(move(value));
  assert(real_integer->get_actual_value() == 67);

  print_success_msg("cast_value with unique ptr", 1);
}

void test_shared_cast_value() {
  shared_ptr<Value> value = make_shared<IntegerValue>(67);
  assert(value->to_string() == "67");

  shared_ptr<IntegerValue> real_integer = cast_value<IntegerValue>(value);
  assert(real_integer->get_actual_value() == 67);
  assert(value != nullptr); // "value" is still accessible
  assert(value.get() == real_integer.get());

  print_success_msg("cast_value with shared ptr", 1);
}

void test_shared_cast_const_value() {
  shared_ptr<const Value> value = make_shared<const IntegerValue>(67);
  assert(value->to_string() == "67");

  shared_ptr<const IntegerValue> real_integer = cast_const_value<IntegerValue>(value);
  assert(real_integer->get_actual_value() == 67);
  assert(value != nullptr); // "value" is still accessible
  assert(value.get() == real_integer.get());

  print_success_msg("cast const value with shared ptr", 1);
}

int main() {
  print_title("Miscellaneous tests...");

  test_instanceof();
  test_replace_substring();
  test_remove_substring();
  test_remove_character();
  test_string_contains();
  test_cast_node();
  test_unique_cast_value();
  test_shared_cast_value();
  test_shared_cast_const_value();

  print_success_msg("All \"Miscellaneous\" tests successfully passed");
  return 0;
}