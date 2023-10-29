#include <iostream>
#include <list>
#include "helper.hpp"
#include "../include/values/compositer.hpp"
#include "../include/symbol_table.hpp"
using namespace std;

void test_set() {
  SymbolTable* table = new SymbolTable();
  IntegerValue* value = new IntegerValue(5);
  table->set("a", value);

  assert(table->exists("a"));
  assert(table->get("a") == value);

  Value* value_from_table = table->get("a");
  IntegerValue* integer_from_table = dynamic_cast<IntegerValue*>(value_from_table);
  assert(integer_from_table != nullptr);
  assert(integer_from_table->get_actual_value() == 5);

  delete table; // will also delete "value"
  print_success_msg("defines variables", 1);
}

void test_casts() {
  SymbolTable* table = new SymbolTable();
  IntegerValue* value = new IntegerValue(5);
  table->set("a", value->copy());

  assert(table->exists("a"));

  Value* value_from_table = table->get("a");
  IntegerValue* integer_from_table = dynamic_cast<IntegerValue*>(value_from_table);
  assert(integer_from_table != nullptr);
  assert(integer_from_table->get_actual_value() == 5);

  delete table; // will also delete "value"
  print_success_msg("defines variables and casts them", 1);
}

void test_remove() {
  SymbolTable* table = new SymbolTable();
  IntegerValue* value = new IntegerValue(5);
  table->set("a", value);

  assert(table->exists("a"));
  assert(table->get("a") == value);

  table->remove("a");
  assert(!table->exists("a"));

  delete table;
  print_success_msg("removes variables", 1);
}

void test_modify() {
  SymbolTable* table = new SymbolTable();
  IntegerValue* value = new IntegerValue(5);
  table->set("a", value);

  assert(table->exists("a"));
  assert(table->get("a") == value);

  IntegerValue* new_value = new IntegerValue(10);
  table->modify("a", new_value);

  assert(table->exists("a"));
  assert(table->get("a") != value);
  assert(table->get("a") == new_value);

  delete value;
  delete table;
  print_success_msg("modifies variables", 1);
}

void test_clear() {
  SymbolTable* table = new SymbolTable();
  IntegerValue* value = new IntegerValue(5);
  table->set("a", value);

  assert(table->exists("a"));
  assert(table->get("a") == value);

  table->clear();
  assert(!table->exists("a"));

  delete table;
  delete value;
  print_success_msg("clears table", 1);
}

void test_nested_tables() {
  SymbolTable* global = new SymbolTable();
  SymbolTable* nested = new SymbolTable(global); // we give as argument the parent symbol table

  IntegerValue* a = new IntegerValue(5);
  nested->set("a", a);

  IntegerValue* constant = new IntegerValue(10);
  global->set("constant", constant);
  
  assert(nested->has_parent());
  assert(!global->has_parent());
  assert(!global->exists("a"));
  assert(!nested->exists("constant"));
  assert(global->exists("constant"));
  assert(nested->exists("a"));
  assert(nested->exists_globally("a"));
  assert(nested->get_highest_parent_context() == global);
  assert(nested->does_constant_exist("constant"));

  delete nested; // deletes its values and all the parent symbol tables
  print_success_msg("works with nested symbol tables", 1);
}

void test_has_value() {
  SymbolTable* global = new SymbolTable();
  SymbolTable* nested = new SymbolTable(global);

  IntegerValue* a = new IntegerValue(5);
  global->set("a", a);

  assert(!nested->exists("a"));
  assert(global->exists("a"));
  assert(nested->has_value_globally(a));
  assert(global->has_value_globally(a));

  delete nested;
  print_success_msg("has value", 1);
}

int main() {
  print_title("SymbolTable tests...");

  test_set();
  test_casts();
  test_remove();
  test_modify();
  test_clear();
  test_nested_tables();
  test_has_value();

  print_success_msg("All \"SymbolTable\" tests successfully passed");
  return 0;
}