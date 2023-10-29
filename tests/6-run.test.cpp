#include <iostream>
#include "helper.hpp"
#include "../include/run.hpp"
#include "../include/context.hpp"
#include "../include/runtime.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/values/compositer.hpp"
using namespace std;

void test_expression() {
  Context* ctx = new Context("tests");

  const string input = "5+5";
  const string filename = "tests";
  const RuntimeResult* res = run(input, filename, ctx);
  assert(res->get_error() == nullptr);
  assert(instanceof<ListValue>(res->get_value()));

  const auto main_value = dynamic_cast<ListValue*>(res->get_value());
  const auto elements = main_value->get_elements();
  assert(instanceof<IntegerValue>(elements->front()));
  auto integer = dynamic_cast<const IntegerValue*>(elements->front());
  assert(integer->get_actual_value() == 10);

  delete ctx;
  delete res;

  print_success_msg("works with simple maths expression", 1);
}

int main() {
  print_title("run() function tests...");

  test_expression();

  print_success_msg("run() tests successfully passed");
  return 0;
}