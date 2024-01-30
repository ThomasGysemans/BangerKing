#include <iostream>
#include "doctest.h"
#include "../include/runtime.hpp"
#include "../include/context.hpp"
#include "../include/values/integer.hpp"
#include "../include/values/list.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/exceptions/runtime_error.hpp"
using namespace std;

DOCTEST_TEST_SUITE("Runtime") {
  SCENARIO("blank runtime") {
    unique_ptr<RuntimeResult> res = make_unique<RuntimeResult>();
    CHECK(res->get_error() == nullptr);
    CHECK(res->get_value() == nullptr);
    CHECK(res->should_return() == false);
  }

  SCENARIO("success") {
    unique_ptr<RuntimeResult> res = make_unique<RuntimeResult>();
    unique_ptr<Value> value = make_unique<IntegerValue>(10);
    res->success(move(value));
    CHECK(res->get_error() == nullptr);
    CHECK(res->get_value()->get_type() == Type::INT);
  }

  SCENARIO("failure") {
    shared_ptr<Context> ctx = make_shared<Context>("<test>");
    unique_ptr<BaseRuntimeError> error = make_unique<RuntimeError>(Position::getDefaultPos(), Position::getDefaultPos(), "test", ctx);
    unique_ptr<RuntimeResult> res = make_unique<RuntimeResult>();
    res->failure(move(error));
    CHECK(res->get_value() == nullptr);
    CHECK(res->get_error() != nullptr);
    CHECK(res->get_error()->get_details() == "test");
    CHECK(res->should_return());
  }

  SCENARIO("read") {
    shared_ptr<Context> ctx = make_shared<Context>("<test>");
    unique_ptr<BaseRuntimeError> error = make_unique<RuntimeError>(Position::getDefaultPos(), Position::getDefaultPos(), "test", ctx);
    unique_ptr<RuntimeResult> top = make_unique<RuntimeResult>();
    unique_ptr<RuntimeResult> bot = make_unique<RuntimeResult>();
    bot->failure(move(error));
    CHECK(bot->get_error() != nullptr);
    CHECK(top->read(move(bot)) == nullptr);
    CHECK(top->get_error() != nullptr);
    CHECK(top->get_value() == nullptr);

    unique_ptr<Value> value = make_unique<IntegerValue>(10);
    top->success(move(value));
    CHECK(top->get_error() == nullptr);
    CHECK(top->get_value() != nullptr);
  }

  SCENARIO("list value") {
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

    CHECK(res->get_value() != nullptr);
    CHECK(res->get_error() == nullptr);
    CHECK(res->get_value()->to_string() == "[" + integer->to_string() + "]");

    // The RuntimeResult should be storing the list_value
    // as an instance of Value, in a shared pointer.
    shared_ptr<Value> value = res->get_value();
    CHECK(value != nullptr);

    // This value should be castable to an instance of ListValue.
    // From there, I can make sure that the process of extracting the value
    // from the RuntimeResult and cast it doesn't create any problems.
    shared_ptr<ListValue> res_list_value = cast_value<ListValue>(value);
    CHECK(res_list_value != nullptr);
    CHECK(res_list_value->get_pos_start()->equals(Position::getDefaultPos()));
    CHECK(res_list_value->get_pos_end()->equals(Position::getDefaultPos()));
    CHECK(res_list_value->get_ctx().get() == ctx.get());
    CHECK(res_list_value->get_type() == Type::LIST);

    // The ListValue contains its values within a list of shared pointers.
    // In this list, there should be the pointer to the "integer" variable.
    list<shared_ptr<const Value>> res_elements = res_list_value->get_elements();
    CHECK(res_elements.size() == 1);
    CHECK(res_elements.front().get() == integer.get());

    // This should be "integer".
    const IntegerValue* res_integer = dynamic_cast<const IntegerValue*>(res_elements.front().get());
    CHECK(res_integer->get_actual_value() == 10);
  }
}