#include <iostream>
#include <list>
#include "helper.hpp"
#include "../include/lexer.hpp"
#include "../include/parser.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/values/compositer.hpp"
#include "../include/utils/deallocate_list_of_pointers.hpp"
using namespace std;

void test_integer() {
  const IntegerValue* integer = new IntegerValue(5);
  const IntegerValue* default_integer = new IntegerValue();
  assert(integer->get_actual_value() == 5);
  assert(integer->to_string() == "5");
  assert(default_integer->get_actual_value() == IntegerValue::get_default_value());
  
  delete integer;
  delete default_integer;

  print_success_msg("integer value", 1);
}

void test_double() {
  const DoubleValue* d = new DoubleValue(3.14);
  const DoubleValue* double_default = new DoubleValue();
  assert(d->get_actual_value() == 3.14);
  assert(d->to_string() == "3.14");
  assert(double_default->get_actual_value() == DoubleValue::get_default_value());

  delete d;
  delete double_default;

  return print_success_msg("double value", 1);
}

void test_copy() {
  const IntegerValue* integer = new IntegerValue(5);
  const IntegerValue* copy = integer->copy();

  const Position* copy_pos_start = copy->get_pos_start();
  const Position* copy_pos_end = copy->get_pos_end();

  assert(integer->get_actual_value() == copy->get_actual_value());
  assert(integer->get_pos_start() != copy_pos_start);
  assert(integer->get_pos_end() != copy_pos_end);
  assert(integer->get_pos_start()->equals(*copy_pos_start));
  assert(integer->get_pos_end()->equals(*copy_pos_end));
  assert(&integer != &copy);

  delete integer;
  delete copy;
  print_success_msg("copy of an integer", 1);
}

int main() {
  print_title("Values tests...");

  test_integer();
  test_double();
  test_copy();

  print_success_msg("All \"Values\" tests successfully passed");

  return 0;
}
