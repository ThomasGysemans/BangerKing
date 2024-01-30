#include <iostream>
#include <list>
#include "doctest.h"
#include "../include/lexer.hpp"
#include "../include/parser.hpp"
#include "../include/types.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/values/compositer.hpp"
using namespace std;

DOCTEST_TEST_SUITE("Values") {
  SCENARIO("copy") {
    unique_ptr<IntegerValue> integer = make_unique<IntegerValue>(5);
    unique_ptr<IntegerValue> copy = unique_ptr<IntegerValue>(integer->copy());

    const Position* copy_pos_start = copy->get_pos_start();
    const Position* copy_pos_end = copy->get_pos_end();

    CHECK(integer->get_actual_value() == copy->get_actual_value());
    CHECK(integer->get_pos_start() != copy_pos_start);
    CHECK(integer->get_pos_end() != copy_pos_end);
    CHECK(integer->get_pos_start()->equals(*copy_pos_start));
    CHECK(integer->get_pos_end()->equals(*copy_pos_end));
    CHECK(integer.get() != copy.get());
  }

  SCENARIO("integer") {
    unique_ptr<IntegerValue> integer = make_unique<IntegerValue>(5);
    unique_ptr<IntegerValue> default_integer = make_unique<IntegerValue>();
    CHECK(integer->get_actual_value() == 5);
    CHECK(integer->to_string() == "5");
    CHECK(integer->is_truthy());
    CHECK(default_integer->get_actual_value() == IntegerValue::get_default_value());
    CHECK(!default_integer->is_truthy());
  }

  SCENARIO("double") {
    unique_ptr<DoubleValue> d = make_unique<DoubleValue>(3.14);
    unique_ptr<DoubleValue> double_default = make_unique<DoubleValue>();
    CHECK(d->get_actual_value() == 3.14);
    CHECK(d->to_string() == "3.14");
    CHECK(d->is_truthy());
    CHECK(double_default->get_actual_value() == DoubleValue::get_default_value());
    CHECK(!double_default->is_truthy());
  }

  SCENARIO("string") {
    // Test on a simple string
    const StringValue hello("hello");
    CHECK(hello.get_actual_value() == "hello");
    CHECK(hello.to_string() == "hello");
    CHECK(hello.is_truthy());

    // Test on an empty string
    const StringValue default_str;
    CHECK(StringValue::get_default_value().empty());
    CHECK(default_str.get_actual_value().empty());
    CHECK(default_str.to_string().empty());
    CHECK(!default_str.is_truthy());

    // Test the possible casts from StringValue to another type
    unique_ptr<IntegerValue> cast_int = cast_value<IntegerValue>(hello.cast(Type::INT));
    CHECK(cast_int->get_actual_value() == (int)(hello.get_actual_value().length()));

    // Test external concatenation
    const StringValue world("world");
    const StringValue* helloworld = hello + world;
    CHECK(helloworld->get_actual_value() == "helloworld");
    CHECK(helloworld->is_truthy());
    delete helloworld;
    // External concatenation with an integer
    const IntegerValue random_integer(10);
    const StringValue* string_with_integer = hello + random_integer;
    CHECK(string_with_integer->get_actual_value() == "hello10");
    delete string_with_integer;
    // External concatenation with a double
    const DoubleValue random_double(3.14);
    const StringValue* string_with_double = hello + random_double;
    CHECK(string_with_double->get_actual_value() == "hello3.14");
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
    CHECK(zzz->get_actual_value() == "zzz");
    CHECK(zzz->is_truthy());
    CHECK(z.is_truthy());
    CHECK(invalid_str == nullptr);
    CHECK(empty->get_actual_value().empty());
    CHECK(hellohellohello->get_actual_value() == "hellohellohello");
    delete zzz;
    delete empty;
    delete invalid_str;
    delete hellohellohello;
  }

  SCENARIO("boolean") {
    const BooleanValue tbool(true);
    CHECK(tbool.is_truthy());
    CHECK(tbool.get_actual_value());
    CHECK(tbool.get_type() == Type::BOOLEAN);

    const BooleanValue fbool(false);
    CHECK(!fbool.is_truthy());
    CHECK(!fbool.get_actual_value());
    CHECK(fbool.get_type() == Type::BOOLEAN);

    const BooleanValue default_bool;
    CHECK(!default_bool.is_truthy());
    CHECK(!default_bool.get_actual_value());
    CHECK(default_bool.get_type() == Type::BOOLEAN);

    CHECK(cast_value<IntegerValue>(tbool.cast(Type::INT))->get_actual_value() == 1);
    CHECK(cast_value<IntegerValue>(fbool.cast(Type::INT))->get_actual_value() == 0);
    CHECK(cast_value<IntegerValue>(default_bool.cast(Type::INT))->get_actual_value() == 0);

    CHECK(cast_value<DoubleValue>(tbool.cast(Type::DOUBLE))->get_actual_value() == 1);
    CHECK(cast_value<DoubleValue>(fbool.cast(Type::DOUBLE))->get_actual_value() == 0);
    CHECK(cast_value<DoubleValue>(default_bool.cast(Type::DOUBLE))->get_actual_value() == 0);

    CHECK(cast_value<StringValue>(tbool.cast(Type::STRING))->get_actual_value() == "1");
    CHECK(cast_value<StringValue>(fbool.cast(Type::STRING))->get_actual_value() == "0");
    CHECK(cast_value<StringValue>(default_bool.cast(Type::STRING))->get_actual_value() == "0");
  }
}