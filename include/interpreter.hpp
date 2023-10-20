#pragma once

#include "runtime.hpp"
#include "context.hpp"
#include "nodes/compositer.hpp"
#include "values/compositer.hpp"
using namespace std;

class Interpreter {
  public:
    RuntimeResult* visit(const CustomNode* node, const Context& ctx) const;
    
  private:
    RuntimeResult* visit_NumberNode(const NumberNode* node, const Context& ctx) const;
    RuntimeResult* visit_ListNode(const ListNode* node, const Context& ctx) const;
    RuntimeResult* visit_AddNode(const AddNode* node, const Context& ctx) const;
    RuntimeResult* visit_SubstractNode(const SubstractNode* node, const Context& ctx) const;

    // helper methods:

    /// @brief Populates a value with its starting and ending position, and its context.
    /// @param value The value to populate.
    /// @param pos_start The starting position to give to the given value.
    /// @param pos_end The ending position to give to the given value.
    /// @param ctx The reference of the context to give to the given value.
    void populate(Value* value, const Position& pos_start, const Position& pos_end, const Context& ctx) const;

    /// @brief Populates a value with its starting and ending position, deduced from the given node, and its context.
    /// @param value The value to populate.
    /// @param node The node that created this value, and whose position must be passed to the given value.
    /// @param ctx The reference of the context to give to the given value.
    void populate(Value* value, const CustomNode* node, const Context& ctx) const;
    
    /// @brief Dynamically casts an derived instance of "Value" to an actual instance of "Value".
    /// @param value The derived value.
    /// @return The same value, but dynamically casted to an instance of "Value".
    Value* to_value(Value* value) const;

    /// @brief Throws a `RuntimeException` for an illegal operation (like "5 + a_function" for example).
    /// @param node The node that created this issue.
    /// @param ctx The context in which this issue happened.
    void illegal_operation(const CustomNode* node, const Context& ctx) const;
};
