#include <iostream>
#include <list>
#include "helper.hpp"
#include "../include/values/compositer.hpp"
#include "../include/symbol_table.hpp"
using namespace std;

void test_set() {
  unique_ptr<SymbolTable> table = make_unique<SymbolTable>();
  unique_ptr<IntegerValue> integer = make_unique<IntegerValue>(5);
  table->set("a", move(integer));

  assert(table->exists("a"));

  print_success_msg("defines variables", 1);
}

void test_get() {
  unique_ptr<SymbolTable> table = make_unique<SymbolTable>();
  unique_ptr<IntegerValue> integer = make_unique<IntegerValue>(5);
  table->set("a", move(integer));

  assert(table->exists("a"));

  // A new unique pointer cannot be created to store the result of the cast
  // because it's still the same memory for both,
  // and creating another unique_ptr for the same memory
  // will make the variable get deallocated twice,
  // at the end of the scope
  unique_ptr<IntegerValue> casted_copy = cast_value<IntegerValue>(table->get("a"));

  assert(casted_copy != nullptr);
  assert(casted_copy->get_actual_value() == 5);

  print_success_msg("copies variables", 1);
}

void test_remove() {
  unique_ptr<SymbolTable> table = make_unique<SymbolTable>();
  unique_ptr<IntegerValue> integer = make_unique<IntegerValue>(5);
  table->set("a", move(integer));

  assert(table->exists("a"));

  table->remove("a");
  assert(!table->exists("a"));
  assert(table->get("a") == nullptr);

  print_success_msg("removes variables", 1);
}

void test_modify() {
  unique_ptr<SymbolTable> table = make_unique<SymbolTable>();
  unique_ptr<IntegerValue> integer = make_unique<IntegerValue>(5);
  table->set("a", move(integer));

  assert(table->exists("a"));

  unique_ptr<IntegerValue> new_value = make_unique<IntegerValue>(10);
  table->modify("a", move(new_value));

  assert(table->exists("a"));
  assert(table->get("a").get() != integer.get());
  assert(table->get("a").get() != new_value.get());
  
  unique_ptr<IntegerValue> new_value_from_table = cast_value<IntegerValue>(table->get("a"));
  assert(new_value_from_table->get_actual_value() == 10);

  print_success_msg("modifies variables", 1);
}

void test_clear() {
  unique_ptr<SymbolTable> table = make_unique<SymbolTable>();
  unique_ptr<IntegerValue> integer = make_unique<IntegerValue>(5);
  table->set("a", move(integer));

  assert(table->exists("a"));

  table->clear();
  assert(!table->exists("a"));
  assert(table->get("a") == nullptr);

  print_success_msg("clears table", 1);
}

void test_nested_tables() {
  shared_ptr<SymbolTable> global = make_shared<SymbolTable>(); // it's shared, so it can be used even after the creation of `nested`
  unique_ptr<SymbolTable> nested = make_unique<SymbolTable>(global); // expects a shared_ptr to another table as parent

  unique_ptr<IntegerValue> a = make_unique<IntegerValue>(5);
  nested->set("a", move(a));

  unique_ptr<IntegerValue> constant = make_unique<IntegerValue>(10);
  global->set("constant", move(constant));
  
  assert(nested->has_parent());
  assert(!global->has_parent());
  assert(!global->exists("a"));
  assert(!nested->exists("constant"));
  assert(global->exists("constant"));
  assert(nested->exists("a"));
  assert(nested->exists_globally("a"));
  assert(nested->exists_globally("constant"));
  assert(nested->get_highest_level_table().get() == global.get());
  assert(nested->does_constant_exist("constant"));

  print_success_msg("works with nested symbol tables", 1);
}

int main() {
  print_title("SymbolTable tests...");

  test_set();
  test_get();
  test_remove();
  test_modify();
  test_clear();
  test_nested_tables();

  print_success_msg("All \"SymbolTable\" tests successfully passed");
  return 0;
}