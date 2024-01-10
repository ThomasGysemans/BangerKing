#include "../include/symbol_table.hpp"

SymbolTable::SymbolTable(SymbolTable* p): parent(p) {}

SymbolTable::~SymbolTable() {
  for (const auto& [key, value] : symbols) {
    delete value;
  }
  symbols.clear();
  delete parent;
}

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

bool SymbolTable::has_value_globally(const Value* value) const {
  for (const auto& [_, v] : symbols) {
    if (value == v) {
      return true;
    }
  }
  if (!has_parent()) return false;
  return parent->has_value_globally(value);
}

Value* SymbolTable::get(const string& name) {
  Value* value = exists(name) ? symbols[name] : nullptr;
  if (value == nullptr && has_parent()) {
    return parent->get(name);
  }
  return value;
}

void SymbolTable::modify(const string& name, Value* new_value) {
  if (exists(name)) {
    symbols[name] = new_value;
  } else {
    if (!has_parent()) return;
    parent->modify(name, new_value);
  }
}

void SymbolTable::set(const string& name, Value* value) {
  symbols[name] = value;
}

void SymbolTable::remove(const string& name) {
  if (exists(name)) {
    delete symbols[name];
    symbols.erase(name);
  } else {
    if (!has_parent()) return;
    parent->remove(name);
  }
}

const SymbolTable* SymbolTable::get_highest_parent_context() const {
  if (!has_parent()) {
    return this;
  } else {
    return parent->get_highest_parent_context();
  }
}

bool SymbolTable::does_constant_exist(const string& name) const {
  return get_highest_parent_context()->exists(name);
}

void SymbolTable::clear() {
  symbols.clear();
}