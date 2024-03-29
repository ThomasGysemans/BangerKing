#include "../include/symbol_table.hpp"
using namespace std;

// without "move(p)" the argument would be copied twice:
// - copied in the argument
// - copied when assigned to the class attribute "parent"
// Here it's copied just once.
SymbolTable::SymbolTable(shared_ptr<SymbolTable> p): parent(move(p)) {}

bool SymbolTable::exists(const string& var_name) const {
  return symbols.contains(var_name);
}

bool SymbolTable::exists_globally(const string& var_name) const {
  if (exists(var_name)) {
    return true;
  } else {
    if (!has_parent()) return false;
    return parent->exists_globally(var_name);
  }
}

bool SymbolTable::has_parent() const {
  return parent != nullptr;
}

unique_ptr<Value> SymbolTable::get(const string& name) {
  unique_ptr<Value> value = exists(name) ? unique_ptr<Value>(symbols[name]->get_copy()) : nullptr;
  if (value == nullptr && has_parent()) {
    return parent->get(name);
  }
  return value;
}

void SymbolTable::modify(const string& name, unique_ptr<Value> new_value) {
  if (exists(name)) {
    symbols[name]->overwrite_value(move(new_value));
  } else {
    if (!has_parent()) return;
    parent->modify(name, move(new_value));
  }
}

void SymbolTable::set(const string& name, unique_ptr<Value> value, bool constant) {
  symbols[name] = make_unique<SymbolTableEntry>(
    move(value),
    constant
  );
}

bool SymbolTable::is_constant(const string& name) const {
  if (exists(name)) {
    return symbols.at(name)->is_constant();
  }
  return false;
}

void SymbolTable::remove(const string& name) {
  if (exists(name)) {
    symbols.erase(name);
  } else {
    if (!has_parent()) return;
    parent->remove(name);
  }
}

shared_ptr<const SymbolTable> SymbolTable::get_highest_level_table() const {
  if (!has_parent()) {
    return shared_from_this();
  } else {
    return parent->get_highest_level_table();
  }
}

bool SymbolTable::does_constant_exist(const string& name) const {
  return get_highest_level_table()->exists(name);
}

void SymbolTable::clear() {
  symbols.clear();
}

SymbolTableEntry::SymbolTableEntry(
  unique_ptr<Value> value,
  bool constant
): value(move(value)), constant(constant) {}

bool SymbolTableEntry::is_constant() const {
  return constant;
}

Value* SymbolTableEntry::get_copy() const {
  return value->copy();
}

void SymbolTableEntry::overwrite_value(unique_ptr<Value> new_value) {
  value.reset();
  value = move(new_value);
}