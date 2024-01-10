#include <iostream>
#include "helper.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/values/compositer.hpp"
using namespace std;

void test_instanceof() {
  IntegerValue* integer = new IntegerValue();
  assert(instanceof<Value>(integer));
  assert(!instanceof<ListValue>(integer));

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

int main() {
  print_title("Miscellaneous tests...");

  test_instanceof();
  test_replace_substring();
  test_remove_substring();
  test_remove_character();
  test_string_contains();

  print_success_msg("All \"Miscellaneous\" tests successfully passed");
  return 0;
}