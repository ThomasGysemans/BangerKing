#pragma once

#include <string>
#include "position.hpp"
using namespace std;

// TODO: uncomment this later, and uncomment symbol_table property because it's necessary for variables
// class SymbolTable;

class Context {
  const string display_name;
  const Context* parent;
  const Position* parent_entry_pos;
  // const SymbolTable* symbol_table = nullptr;

  public:
    const string get_display_name() const;
    const Position* get_parent_entry_pos() const;
    const Context* get_parent() const;

    virtual ~Context();

    bool is_context_in(const string& name) const;

    Context(
      const string& name,
      const Context* p_ctx = nullptr,
      const Position* entry_pos = nullptr
    );
};


// const Context* init_global_context(const string& custom_name);
