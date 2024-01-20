#pragma once

#include <string>
#include "position.hpp"
using namespace std;

// Forward declaration's necessary to avoid circular dependency issue.
// It is only possible because the header of "Context" only uses pointers to "SymbolTable".
class SymbolTable;

/// @brief This class holds the symbol table of a context.
/// Its symbol table is automatically deleted when out of reach.
/// It acts as an automatic garbage collector.
/// Note that a context is like a reversed linked list, where each context has a link to its parent,
/// but the context itself has no information whatsoever about the other contexts created within it.
/// The global context shall have a nullptr to its parent.
///
/// An empty symbol table is created when a context is instantiated.
class Context {
  const string display_name;
  shared_ptr<const Context> parent;
  shared_ptr<const Position> parent_entry_pos;
  shared_ptr<SymbolTable> symbol_table;

  public:
    const string get_display_name() const;
    shared_ptr<const Context> get_parent() const;
    shared_ptr<const Position> get_parent_entry_pos() const;
    shared_ptr<SymbolTable> get_symbol_table() const;

    ~Context() = default;

    /// @brief Checks if this context is contained in another context of the given name.
    /// @param name The name of a potential parent context.
    /// @return `true` if there is a parent context with the given name.
    bool is_context_in(const string& name) const;

    /// @brief Creates a new context of the given name. If no parent context is given, this context will be global.
    /// @param name The name of this context (generally the name of the file, or the name of a function).
    /// @param p_ctx A pointer to a parent context.
    /// @param entry_pos The entry position in the program of this context.
    Context(
      const string& name,
      shared_ptr<const Context> p_ctx = nullptr,
      shared_ptr<const Position> entry_pos = nullptr
    );
};
