#pragma once

#include <memory>
#include <map>
#include <any>
#include "miscellaneous.hpp"
#include "values/value.hpp"
using namespace std;

class SymbolTableEntry {
  unique_ptr<Value> value;
  bool constant;

  public:
    SymbolTableEntry(
      unique_ptr<Value>, // move the owernship of the value to this class
      bool // whether it's a constant or not
    );

    /// @brief Gets a copy of the value this entry is holding.
    Value* get_copy() const;

    /// @brief Is the stored value a constant?
    bool is_constant() const;

    /// @brief Releases the current value so as to replace it with the new one.
    /// @param new_value The new value to hold.
    void overwrite_value(unique_ptr<Value> new_value);
};

class SymbolTable: public enable_shared_from_this<SymbolTable> {
  shared_ptr<SymbolTable> parent;
  map<string, unique_ptr<SymbolTableEntry>> symbols;

  public:
    SymbolTable(shared_ptr<SymbolTable> p = nullptr);

    ~SymbolTable() = default;
    
    /// @brief Checks if the symbol table contains a variable named "var_name"
    /// @param var_name The name of the variable.
    /// @return `true` if the given variable exists in this symbol table.
    bool exists(const string& var_name) const;

    /// @brief Checks recursively if a variable exists in this context, or any parent context.
    /// @param var_name The name of the variable.
    /// @return `true` if the given variable exists.
    bool exists_globally(const string& var_name) const;

    /// @brief Checks if this symbol table has a parent context.
    /// @return `true` if there is a parent context.
    bool has_parent() const;

    /// @brief Returns a copy of the value of a variable contained in this symbol table, or in the table of a parent context.
    /// @param name The name of the variable to look for.
    /// @return The value of the given variable, or `nullptr` if it doesn't exist.
    unique_ptr<Value> get(const string& name);

    /// @brief Creates or modifies a variable from the current context, or the parent contexts.
    /// @param name The name of the variable to modify.
    /// @param new_value The value to give to this variable.
    void modify(const string& name, unique_ptr<Value> new_value);

    /// @brief Sets the variable `name` to `value`. It doesn't check if the variable already exists, and only creates the variable in this context.
    /// @param name The name of the variable to create in this current context.
    /// @param value The value to give to this variable.
    /// @param constant Whether this value is a constant (created with the "define" keyword).
    void set(const string& name, unique_ptr<Value> value, bool constant);

    /// @brief Checks if the given variable is a constant.
    /// If this value doesn't exist, it will return `false`.
    /// @param name The name of the variable.
    /// @return `true` if the variable exists and is a constant.
    bool is_constant(const string& name) const;

    /// @brief Removes a variable from this context or a parent context and deallocates the memory.
    /// @param name The name of the variable to remove.
    void remove(const string& name);

    /// @brief Gets the symbol table of the highest parent context.
    /// @return The const instance of the symbol table from the highest parent context.
    shared_ptr<const SymbolTable> get_highest_level_table() const;

    /// @brief Checks if a variable exists in the highest context.
    /// @param name The variable's name.
    /// @return `true` if the given variable exists in the highest context.
    bool does_constant_exist(const string& name) const;

    /// @brief Clears the current context of all its variables.
    void clear();
};