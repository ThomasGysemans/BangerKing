#include <iostream>
#include <fstream>
#include <cstdio>
#include "doctest.h"
#include "../include/run.hpp"
#include "../include/context.hpp"
#include "../include/runtime.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/values/compositer.hpp"
using namespace std;

DOCTEST_TEST_SUITE("run method") {
  SCENARIO("cli") {
    shared_ptr<Context> ctx = make_shared<Context>("<tests>");

    const string input = "5+5";
    unique_ptr<const RuntimeResult> res = runLine(input, ctx);
    CHECK(*READ_FILES["<stdin>"] == input);
    CHECK(res->get_value() != nullptr);
    CHECK(res->get_error() == nullptr);
    shared_ptr<Value> res_value = res->get_value();
    shared_ptr<ListValue> list_value = cast_value<ListValue>(res_value);

    list<shared_ptr<const Value>> elements = list_value->get_elements();
    shared_ptr<const Value> front = elements.front();
    shared_ptr<const IntegerValue> integer = cast_const_value<IntegerValue>(front);
    CHECK(integer->get_actual_value() == 10);
  }

  SCENARIO("file") {
    shared_ptr<Context> ctx = make_shared<Context>("<tests>");

    const string input = "6+6";
    const char* test_filename = "tests_runfile.bk";
    ofstream file = ofstream(test_filename);
    CHECK(file.is_open());
    file << input;
    file.close();

    unique_ptr<const RuntimeResult> res = runFile(test_filename, ctx);
    CHECK(res != nullptr);
    CHECK(res->get_value() != nullptr);
    CHECK(res->get_error() == nullptr);
    shared_ptr<Value> res_value = res->get_value();
    shared_ptr<ListValue> list_value = list_value = cast_value<ListValue>(res_value);

    list<shared_ptr<const Value>> elements = list_value->get_elements();
    shared_ptr<const Value> front = elements.front();
    shared_ptr<const IntegerValue> integer = cast_const_value<IntegerValue>(front);
    CHECK(integer->get_actual_value() == 12);

    remove(test_filename);
  }
}
