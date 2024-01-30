#include <iostream>
#include <list>
#include "doctest.h"
#include "../include/values/compositer.hpp"
#include "../include/symbol_table.hpp"
using namespace std;

DOCTEST_TEST_SUITE("Symbol Table") {
  SCENARIO("set") {
    unique_ptr<SymbolTable> table = make_unique<SymbolTable>();
    unique_ptr<IntegerValue> integer = make_unique<IntegerValue>(5);
    table->set("a", move(integer), false);

    CHECK(table->exists("a"));
    CHECK(!table->is_constant("a"));
  }

  SCENARIO("set constant") {
    unique_ptr<SymbolTable> table = make_unique<SymbolTable>();
    unique_ptr<IntegerValue> integer = make_unique<IntegerValue>(5);
    table->set("a", move(integer), true);

    CHECK(table->exists("a"));
    CHECK(table->is_constant("a"));
  }

  SCENARIO("get") {
    unique_ptr<SymbolTable> table = make_unique<SymbolTable>();
    unique_ptr<IntegerValue> integer = make_unique<IntegerValue>(5);
    table->set("a", move(integer), false);

    CHECK(table->exists("a"));

    // A new unique pointer cannot be created to store the result of the cast
    // because it's still the same memory for both,
    // and creating another unique_ptr for the same memory
    // will make the variable get deallocated twice,
    // at the end of the scope
    unique_ptr<IntegerValue> casted_copy = cast_value<IntegerValue>(table->get("a"));

    CHECK(casted_copy != nullptr);
    CHECK(casted_copy->get_actual_value() == 5);
  }

  SCENARIO("remove") {
    unique_ptr<SymbolTable> table = make_unique<SymbolTable>();
    unique_ptr<IntegerValue> integer = make_unique<IntegerValue>(5);
    table->set("a", move(integer), false);

    CHECK(table->exists("a"));

    table->remove("a");
    CHECK(!table->exists("a"));
    CHECK(table->get("a") == nullptr);
  }

  SCENARIO("modify") {
    unique_ptr<SymbolTable> table = make_unique<SymbolTable>();
    unique_ptr<IntegerValue> integer = make_unique<IntegerValue>(5);
    table->set("a", move(integer), false);

    CHECK(table->exists("a"));

    unique_ptr<IntegerValue> new_value = make_unique<IntegerValue>(10);
    table->modify("a", move(new_value));

    CHECK(table->exists("a"));
    CHECK(table->get("a").get() != integer.get());
    CHECK(table->get("a").get() != new_value.get());
    
    unique_ptr<IntegerValue> new_value_from_table = cast_value<IntegerValue>(table->get("a"));
    CHECK(new_value_from_table->get_actual_value() == 10);
  }

  SCENARIO("clear") {
    unique_ptr<SymbolTable> table = make_unique<SymbolTable>();
    unique_ptr<IntegerValue> integer = make_unique<IntegerValue>(5);
    table->set("a", move(integer), false);

    CHECK(table->exists("a"));

    table->clear();
    CHECK(!table->exists("a"));
    CHECK(table->get("a") == nullptr);
  }

  SCENARIO("nested tables") {
    shared_ptr<SymbolTable> global = make_shared<SymbolTable>(); // it's shared, so it can be used even after the creation of `nested`
    unique_ptr<SymbolTable> nested = make_unique<SymbolTable>(global); // expects a shared_ptr to another table as parent

    unique_ptr<IntegerValue> a = make_unique<IntegerValue>(5);
    nested->set("a", move(a), false);

    unique_ptr<IntegerValue> constant = make_unique<IntegerValue>(10);
    global->set("constant", move(constant), false);
    
    CHECK(nested->has_parent());
    CHECK(!global->has_parent());
    CHECK(!global->exists("a"));
    CHECK(!nested->exists("constant"));
    CHECK(global->exists("constant"));
    CHECK(nested->exists("a"));
    CHECK(nested->exists_globally("a"));
    CHECK(nested->exists_globally("constant"));
    CHECK(nested->get_highest_level_table().get() == global.get());
    CHECK(nested->does_constant_exist("constant"));
  }
}