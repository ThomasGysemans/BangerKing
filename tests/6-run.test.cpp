#include <iostream>
#include "doctest.h"
#include "../include/run.hpp"
#include "../include/context.hpp"
#include "../include/runtime.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/values/compositer.hpp"
using namespace std;

DOCTEST_TEST_SUITE("run method") {
  SCENARIO("expression") {
    shared_ptr<Context> ctx = make_shared<Context>("<tests>");

    const string input = "5+5";
    const string filename = "tests";
    unique_ptr<const RuntimeResult> res = run(input, filename, ctx);
    CHECK(res->get_value() != nullptr);
    CHECK(res->get_error() == nullptr);
    shared_ptr<Value> res_value = res->get_value();
    shared_ptr<ListValue> list_value = cast_value<ListValue>(res_value);

    list<shared_ptr<const Value>> elements = list_value->get_elements();
    shared_ptr<const Value> front = elements.front();
    shared_ptr<const IntegerValue> integer = cast_const_value<IntegerValue>(front);
    CHECK(integer->get_actual_value() == 10);
  }
}
