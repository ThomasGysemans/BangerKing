#pragma once

#include <map>
#include <any>
#include "miscellaneous.hpp"
#include "values/value.hpp"
using namespace std;

class SymbolTable {
  SymbolTable* parent;
  map<string, Value<>*> symbols;

  public:
    SymbolTable(SymbolTable* p = nullptr);

    virtual ~SymbolTable();
    
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

    /// @brief Returns the value of a variable contained in this symbol table, or in the table of a parent context.
    /// @tparam T The type of the expected value, or `any` by default.
    /// @param name The name of the variable to look for.
    /// @return The value of the given variable, or `nullptr` if it doesn't exist.
    template <typename T = any>
    Value<T>* get(const string& name);

    /// @brief Creates or modifies a variable from the current context, or the parent contexts.
    /// @param name The name of the variable to create/modify.
    /// @param new_value The value to give to this variable.
    void modify(const string& name, Value<>* new_value);

    /// @brief Sets the variable `name` to `value`. It doesn't check if the variable already exists, and only creates the variable in this context.
    /// @param name The name of the variable to create/modify in this current context.
    /// @param value The value to give to this variable.
    void set(const string& name, Value<>* value);

    /// @brief Removes a variable from this context or a parent context and deallocates the memory.
    /// @param name The name of the variable to remove.
    void remove(const string& name);

    /// @brief Gets the highest parent context.
    /// @return The const instance of the highest parent context.
    const SymbolTable* get_highest_parent_context() const;

    /// @brief Checks if a variable exists in the highest context.
    /// @param name The variable's name.
    /// @return `true` if the given variable exists in the highest context.
    bool does_constant_exist(const string& name) const;

    /// @brief Clears the current context of all its variables.
    void clear();
};