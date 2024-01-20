#include <iostream>
#include "helper.hpp"
#include "../include/runtime.hpp"
#include "../include/context.hpp"
#include "../include/values/integer.hpp"
#include "../include/values/list.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/exceptions/runtime_error.hpp"
using namespace std;

void test_blank_runtime() {
  unique_ptr<RuntimeResult> res = make_unique<RuntimeResult>();
  assert(res->get_error() == nullptr);
  assert(res->get_value() == nullptr);
  assert(res->should_return() == false);
  
  print_success_msg("blank runtime", 1);
}

void test_success() {
  unique_ptr<RuntimeResult> res = make_unique<RuntimeResult>();
  unique_ptr<Value> value = make_unique<IntegerValue>(10);
  assert(res->success(move(value)) == res.get());
  assert(res->get_error() == nullptr);
  assert(res->get_value()->get_type() == Type::INT);
  
  print_success_msg("success", 1);
}

void test_failure() {
  shared_ptr<Context> ctx = make_shared<Context>("<test>");
  unique_ptr<BaseRuntimeError> error = make_unique<RuntimeError>(Position::getDefaultPos(), Position::getDefaultPos(), "test", ctx);
  unique_ptr<RuntimeResult> res = make_unique<RuntimeResult>();
  assert(res->failure(move(error)) == res.get());
  assert(res->get_value() == nullptr);
  assert(res->get_error() != nullptr);
  assert(res->get_error()->get_details() == "test");
  assert(res->should_return());
  
  print_success_msg("failure", 1);
}

void test_read() {
  shared_ptr<Context> ctx = make_shared<Context>("<test>");
  unique_ptr<BaseRuntimeError> error = make_unique<RuntimeError>(Position::getDefaultPos(), Position::getDefaultPos(), "test", ctx);
  unique_ptr<RuntimeResult> top = make_unique<RuntimeResult>();
  unique_ptr<RuntimeResult> bot = make_unique<RuntimeResult>();
  bot->failure(move(error));
  assert(bot->get_error() != nullptr);
  assert(top->read(move(bot)) == nullptr);
  assert(top->get_error() != nullptr);
  assert(top->get_value() == nullptr);

  unique_ptr<Value> value = make_unique<IntegerValue>(10);
  top->success(move(value));
  assert(top->get_error() == nullptr);
  assert(top->get_value() != nullptr);

  print_success_msg("reads", 1);
}

void test_list_value() {
  // NOTE: initializing the unique pointer is mandatory
  // Doing the following is problematic:
  // unique_ptr<RuntimeResult> res;
  unique_ptr<RuntimeResult> res = make_unique<RuntimeResult>();
  shared_ptr<Context> ctx = make_shared<Context>("<tests>");
  shared_ptr<IntegerValue> integer = make_shared<IntegerValue>(10);
  list<shared_ptr<const Value>> elements;
  elements.push_back(integer);

  // Creating the ListValue instance
  // And populating it with positions & context
  unique_ptr<ListValue> list_value = make_unique<ListValue>(elements);
  list_value->set_pos(Position::getDefaultPos(), Position::getDefaultPos());
  list_value->set_ctx(ctx);

  // Transferring ownership of the ListValue
  // to the RuntimeResult.
  res->success(move(list_value));

  assert(res->get_value() != nullptr);
  assert(res->get_error() == nullptr);
  assert(res->get_value()->to_string() == "[" + integer->to_string() + "]");

  // The RuntimeResult should be storing the list_value
  // as an instance of Value, in a shared pointer.
  shared_ptr<Value> value = res->get_value();
  assert(value != nullptr);

  // This value should be castable to an instance of ListValue.
  // From there, I can make sure that the process of extracting the value
  // from the RuntimeResult and cast it doesn't create any problems.
  shared_ptr<ListValue> res_list_value = cast_value<ListValue>(value);
  assert(res_list_value != nullptr);
  assert(res_list_value->get_pos_start()->equals(Position::getDefaultPos()));
  assert(res_list_value->get_pos_end()->equals(Position::getDefaultPos()));
  assert(res_list_value->get_ctx().get() == ctx.get());
  assert(res_list_value->get_type() == Type::LIST);

  // The ListValue contains its values within a list of shared pointers.
  // In this list, there should be the pointer to the "integer" variable.
  list<shared_ptr<const Value>> res_elements = res_list_value->get_elements();
  assert(res_elements.size() == 1);
  assert(res_elements.front().get() == integer.get());

  // This should be "integer".
  const IntegerValue* res_integer = dynamic_cast<const IntegerValue*>(res_elements.front().get());
  assert(res_integer->get_actual_value() == 10);

  print_success_msg("accepts a populated ListValue", 1);
}

int main() {
  print_title("Runtime tests...");

  test_blank_runtime();
  test_success();
  test_failure();
  test_read();
  test_list_value();

  print_success_msg("All \"Runtime\" tests successfully passed");
  return 0;
}