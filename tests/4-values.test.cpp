#include <iostream>
#include <list>
#include "helper.hpp"
#include "../include/lexer.hpp"
#include "../include/parser.hpp"
#include "../include/types.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/values/compositer.hpp"
using namespace std;

void test_copy() {
  unique_ptr<IntegerValue> integer = make_unique<IntegerValue>(5);
  unique_ptr<IntegerValue> copy = unique_ptr<IntegerValue>(integer->copy());

  const Position* copy_pos_start = copy->get_pos_start();
  const Position* copy_pos_end = copy->get_pos_end();

  assert(integer->get_actual_value() == copy->get_actual_value());
  assert(integer->get_pos_start() != copy_pos_start);
  assert(integer->get_pos_end() != copy_pos_end);
  assert(integer->get_pos_start()->equals(*copy_pos_start));
  assert(integer->get_pos_end()->equals(*copy_pos_end));
  assert(integer.get() != copy.get());

  print_success_msg("copy of an integer", 1);
}

void test_integer() {
  unique_ptr<IntegerValue> integer = make_unique<IntegerValue>(5);
  unique_ptr<IntegerValue> default_integer = make_unique<IntegerValue>();
  assert(integer->get_actual_value() == 5);
  assert(integer->to_string() == "5");
  assert(integer->is_truthy());
  assert(default_integer->get_actual_value() == IntegerValue::get_default_value());
  assert(!default_integer->is_truthy());
  
  print_success_msg("integer value", 1);
}

void test_double() {
  unique_ptr<DoubleValue> d = make_unique<DoubleValue>(3.14);
  unique_ptr<DoubleValue> double_default = make_unique<DoubleValue>();
  assert(d->get_actual_value() == 3.14);
  assert(d->to_string() == "3.14");
  assert(d->is_truthy());
  assert(double_default->get_actual_value() == DoubleValue::get_default_value());
  assert(!double_default->is_truthy());

  print_success_msg("double value", 1);
}

void test_string() {
  // Test on a simple string
  const StringValue hello("hello");
  assert(hello.get_actual_value() == "hello");
  assert(hello.to_string() == "hello");
  assert(hello.is_truthy());

  // Test on an empty string
  const StringValue default_str;
  assert(StringValue::get_default_value().empty());
  assert(default_str.get_actual_value().empty());
  assert(default_str.to_string().empty());
  assert(!default_str.is_truthy());

  // Test the possible casts from StringValue to another type
  unique_ptr<IntegerValue> cast_int = cast_value<IntegerValue>(hello.cast(Type::INT));
  assert(cast_int->get_actual_value() == hello.get_actual_value().length());

  // Test external concatenation
  const StringValue world("world");
  const StringValue* helloworld = hello + world;
  assert(helloworld->get_actual_value() == "helloworld");
  assert(helloworld->is_truthy());
  delete helloworld;
  // External concatenation with an integer
  const IntegerValue random_integer(10);
  const StringValue* string_with_integer = hello + random_integer;
  assert(string_with_integer->get_actual_value() == "hello10");
  delete string_with_integer;
  // External concatenation with a double
  const DoubleValue random_double(3.14);
  const StringValue* string_with_double = hello + random_double;
  assert(string_with_double->get_actual_value() == "hello3.14");
  delete string_with_double;

  // Repeat a string 3 times
  const IntegerValue x(3);
  const IntegerValue zero;
  const IntegerValue negative(-1);
  const StringValue z("z");
  const StringValue* zzz = z * x;
  const StringValue* invalid_str = hello * negative;
  const StringValue* empty = hello * zero;
  const StringValue* hellohellohello = hello * x;
  assert(zzz->get_actual_value() == "zzz");
  assert(zzz->is_truthy());
  assert(z.is_truthy());
  assert(invalid_str == nullptr);
  assert(empty->get_actual_value().empty());
  assert(hellohellohello->get_actual_value() == "hellohellohello");
  delete zzz;
  delete empty;
  delete invalid_str;
  delete hellohellohello;

  print_success_msg("string value", 1);
}

int main() {
  print_title("Values tests...");

  test_copy();
  test_integer();
  test_double();
  test_string();

  print_success_msg("All \"Values\" tests successfully passed");

  return 0;
}
