#pragma once

#include <array>
#include "runtime.hpp"
#include "context.hpp"
#include "nodes/compositer.hpp"
#include "values/compositer.hpp"
using namespace std;

class Interpreter {
  static Context* shared_ctx;
  
  public:
    static void set_shared_ctx(Context* ctx);
    static RuntimeResult* visit(const CustomNode* node);
    
  private:
    static RuntimeResult* visit_IntegerNode(const IntegerNode* node);
    static RuntimeResult* visit_ListNode(const ListNode* node);
    static RuntimeResult* visit_AddNode(const AddNode* node);
    static RuntimeResult* visit_SubstractNode(const SubstractNode* node);
    static RuntimeResult* visit_MultiplyNode(const MultiplyNode* node);
    static RuntimeResult* visit_PowerNode(const PowerNode* node);
    static RuntimeResult* visit_DivideNode(const DivideNode* node);
    static RuntimeResult* visit_ModuloNode(const ModuloNode* node);
    static RuntimeResult* visit_MinusNode(const MinusNode* node);
    static RuntimeResult* visit_PlusNode(const PlusNode* node);
    static RuntimeResult* visit_VarAssignmentNode(const VarAssignmentNode* node);
    static RuntimeResult* visit_VarAccessNode(const VarAccessNode* node);
    static RuntimeResult* visit_VarModifyNode(const VarModifyNode* node);

    // helper methods:
    
    /// @brief Populates a value with its starting and ending position, and its context.
    /// @param value The value to populate.
    /// @param pos_start The starting position to give to the given value.
    /// @param pos_end The ending position to give to the given value.
    /// @param ctx The reference of the context to give to the given value.
    static void populate(Value* value, const Position& pos_start, const Position& pos_end, const Context* ctx);

    /// @brief Populates a value with its starting and ending position, deduced from the given node, and its context.
    /// @param value The value to populate.
    /// @param node The node that created this value, and whose position must be passed to the given value.
    /// @param ctx The reference of the context to give to the given value.
    static void populate(Value* value, const CustomNode* node, const Context* ctx);
    
    /// @brief Dynamically casts a derived instance of "Value" to an actual instance of "Value".
    /// @param value The derived value.
    /// @return The same value, but dynamically casted to an instance of "Value".
    static Value* to_value(Value* value);

    /// @brief Throws a `RuntimeError` for an illegal operation (like "5 + a_function" for example).
    /// @param node The node that created this issue.
    /// @param ctx The context in which this issue happened.
    static void illegal_operation(const CustomNode* node, const Context* ctx);

    /// @brief Throws a `TypeError` for trying to assign an incompatible type to a variable.
    /// @param value The value whose type differs from the `expected_type`.
    /// @param expected_type The type of the variable.
    /// @param ctx The context in which the error occured.
    static void type_error(const Value* value, const Type expected_type, const Context* ctx);

    /// @brief Makes sure that the type of the given value is compatible with the type of the variable whose name is `variable_name` by throwing a `TypeError` if they're not.
    /// @param new_value The value that we want to assign to a variable.
    /// @param expected_type The type that `new_value` should have, or at least be compatible with.
    /// @param ctx The context in which to find the variable.
    static void ensure_type_compatibility(const Value* new_value, const Type expected_type, const Context* ctx);

    /// @brief Makes sure that the type of the given value is compatible with the type of the variable whose name is `variable_name` by throwing a `TypeError` if they're not.
    /// @param new_value The value that we want to assign to a variable.
    /// @param variable_name The name of the variable.
    /// @param ctx The context in which to find the variable.
    static void ensure_type_compatibility(const Value* new_value, const string variable_name, const Context* ctx);
};
