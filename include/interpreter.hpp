#pragma once

#include <cmath>
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
    static RuntimeResult* visit_DoubleNode(const DoubleNode* node);
    static RuntimeResult* visit_ListNode(const ListNode* node);
    static RuntimeResult* visit_MinusNode(const MinusNode* node);
    static RuntimeResult* visit_PlusNode(const PlusNode* node);
    static RuntimeResult* visit_VarAssignmentNode(const VarAssignmentNode* node);
    static RuntimeResult* visit_VarAccessNode(const VarAccessNode* node);
    static RuntimeResult* visit_VarModifyNode(const VarModifyNode* node);
    static RuntimeResult* visit_StringNode(const StringNode* node);

    /// @brief Explores a binary operation node (addition, substraction, division, power, multiplication, modulo, etc.)
    /// @param node A binary operation node.
    static RuntimeResult* visit_BinaryOperationNode(const BinaryOperationNode* node);
    static Value* interpret_addition(const Value* left, const Value* right, const BinaryOperationNode* node);
    static Value* interpret_substraction(const Value* left, const Value* right, const BinaryOperationNode* node);
    static Value* interpret_multiplication(const Value* left, const Value* right, const BinaryOperationNode* node);
    static Value* interpret_power(const Value* left, const Value* right, const BinaryOperationNode* node);
    static Value* interpret_division(const Value* left, const Value* right, const BinaryOperationNode* node);
    static Value* interpret_modulo(const Value* left, const Value* right, const BinaryOperationNode* node);

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
    /// @return The same value, but dynamically cast to an instance of "Value".
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

    /// @brief Applies a binary mathematical operation between `left` and `right` during the interpretation of `node`.
    /// The operation to apply is given as a lambda function via the `operation` argument.
    /// This method will populate the new value with the positions of the provided `node` and the `shared_ctx`.
    /// This method also deallocates the memory used by `left` and `right`.
    /// @tparam A The exact type of the left member.
    /// @tparam B The exact type of the right member.
    /// @tparam Op The lambda function that's automatically deduced when calling this function. No need to specify it explicitely.
    /// @param left The left member of the operation.
    /// @param right The right member of the operation.
    /// @param node The binary operation node that the Interpreter is currently interpreting.
    /// @return An instance of `Value` from the operation.
    template <typename A, typename B, typename Op>
    static Value* make_operation(const Value* left, const Value* right, const BinaryOperationNode* node, Op operation) {
      const A* a = dynamic_cast<const A*>(left);
      const B* b = dynamic_cast<const B*>(right);
      auto r = operation(*a, *b);
      if (r == nullptr) {
        // It's possible in some cases that during the operation
        // it fails and needs to throw a RuntimeError.
        // It happens in this example: string * int (if int is negative).
        // Therefore I delete the values, and the node should get deallocated separately.
        delete a;
        delete b;
        delete r;
        illegal_operation(node, shared_ctx);
      }
      populate(r, node, shared_ctx);
      delete a;
      delete b;
      return to_value(r);
    }

    /// @brief Applies an addition between `left` and `right`.
    template <typename A, typename B>
    static Value* make_addition(const Value* left, const Value* right, const BinaryOperationNode* node) {
      return make_operation<A, B>(left, right, node, [](const A& a, const B& b) { return a + b; });
    }

    /// @brief Applies a substraction between `left` and `right`.
    template <typename A, typename B>
    static Value* make_substraction(const Value* left, const Value* right, const BinaryOperationNode* node) {
      return make_operation<A, B>(left, right, node, [](const A& a, const B& b) { return a - b; });
    }

    /// @brief Applies a multiplication between `left` and `right`.
    template <typename A, typename B>
    static Value* make_multiplication(const Value* left, const Value* right, const BinaryOperationNode* node) {
      return make_operation<A, B>(left, right, node, [](const A& a, const B& b) { return a * b; });
    }

    /// @brief Applies a power operation between `left` and `right`.
    /// @tparam R Since the result type cannot be deduced, it must be specified when calling this method.
    template <typename A, typename B, typename R>
    static Value* make_power(const Value* left, const Value* right, const BinaryOperationNode* node) {
      return make_operation<A, B>(left, right, node, [](const A& a, const B& b) { return new R(std::pow(a.get_actual_value(), b.get_actual_value())); });
    }

    /// @brief Applies a division between `left` and `right`.
    template <typename A, typename B>
    static Value* make_division(const Value* left, const Value* right, const BinaryOperationNode* node) {
      return make_operation<A, B>(left, right, node, [](const A& a, const B& b) { return a / b; });
    }

    /// @brief Applies a modulo between `left` and `right`.
    template <typename A, typename B>
    static Value* make_modulo(const Value* left, const Value* right, const BinaryOperationNode* node) {
      return make_operation<A, B>(left, right, node, [](const A& a, const B& b) { return a % b; });
    }
};
