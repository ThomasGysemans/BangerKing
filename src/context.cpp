#include "../include/context.hpp"
#include "../include/symbol_table.hpp"

const string Context::get_display_name() const { return display_name; }
shared_ptr<const Context> Context::get_parent() const { return parent; }
shared_ptr<const Position> Context::get_parent_entry_pos() const { return parent_entry_pos; }
shared_ptr<SymbolTable> Context::get_symbol_table() const { return symbol_table; }

Context::Context(
  const string& name,
  shared_ptr<const Context> p_ctx,
  shared_ptr<const Position> entry_pos
): display_name(name), parent(p_ctx), parent_entry_pos(entry_pos) {
  symbol_table = make_shared<SymbolTable>();
}

bool Context::is_context_in(const string& name) const {
  if (display_name == name) return true;
  if (parent == nullptr) return false;
  return parent->is_context_in(name);
}