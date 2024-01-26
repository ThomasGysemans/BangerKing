#pragma once

#include <cmath>
#include <array>
#include "runtime.hpp"
#include "context.hpp"
#include "nodes/compositer.hpp"
#include "values/compositer.hpp"
using namespace std;

class Interpreter {
  static shared_ptr<Context> shared_ctx;
  
  public:
    static void set_shared_ctx(shared_ptr<Context> ctx);

    /// @brief Interprets a node recursively.
    /// The very first node to give to the interpreter should be a ListNode,
    /// even though it would work with any other kind of a node.
    /// A program is a list of nodes, so it makes sense to pass a ListNode.
    /// 
    /// So as to improve memory management, the ownership of the nodes
    /// must be transferred to the visit methods, so as to
    /// deallocate them automatically when they've been visited.
    /// The benefit of doing this is that the memory used by the Parser,
    /// is deallocated progressively while the Interpreter's doing its magic.
    /// @param node The node to interpret
    /// @return The result of the intepretation
    static unique_ptr<RuntimeResult> visit(unique_ptr<CustomNode> node);
    
  private:
    // Here the specific visit methods.
    // Each of them will take care of interpreting a specific node.
    // They also become the owners of the node they're interpreting,
    // so as to deallocate the memory used by the Parser progressively.
    // They become the owners because the parameter is a unique_ptr.

    static unique_ptr<RuntimeResult> visit_IntegerNode(unique_ptr<const IntegerNode> node);
    static unique_ptr<RuntimeResult> visit_DoubleNode(unique_ptr<const DoubleNode> node);
    static unique_ptr<RuntimeResult> visit_ListNode(unique_ptr<ListNode> node);
    static unique_ptr<RuntimeResult> visit_MinusNode(unique_ptr<MinusNode> node);
    static unique_ptr<RuntimeResult> visit_PlusNode(unique_ptr<PlusNode> node);
    static unique_ptr<RuntimeResult> visit_VarAssignmentNode(unique_ptr<VarAssignmentNode> node);
    static unique_ptr<RuntimeResult> visit_VarAccessNode(unique_ptr<VarAccessNode> node);
    static unique_ptr<RuntimeResult> visit_VarModifyNode(unique_ptr<VarModifyNode> node);
    static unique_ptr<RuntimeResult> visit_StringNode(unique_ptr<const StringNode> node);
    static unique_ptr<RuntimeResult> visit_BooleanNode(unique_ptr<const BooleanNode> node);

    /// @brief Explores a binary operation node (addition, substraction, division, power, multiplication, modulo, etc.)
    /// @param node A binary operation node.
    /// @return The intepretation of this operation as a RuntimeResult.
    static unique_ptr<RuntimeResult> visit_BinaryOperationNode(unique_ptr<BinaryOperationNode> node);

    static unique_ptr<Value> interpret_addition(shared_ptr<const Value> left, shared_ptr<const Value> right, unique_ptr<const BinaryOperationNode> node);
    static unique_ptr<Value> interpret_substraction(shared_ptr<const Value> left, shared_ptr<const Value> right, unique_ptr<const BinaryOperationNode> node);
    static unique_ptr<Value> interpret_multiplication(shared_ptr<const Value> left, shared_ptr<const Value> right, unique_ptr<const BinaryOperationNode> node);
    static unique_ptr<Value> interpret_power(shared_ptr<const Value> left, shared_ptr<const Value> right, unique_ptr<const BinaryOperationNode> node);
    static unique_ptr<Value> interpret_division(shared_ptr<const Value> left, shared_ptr<const Value> right, unique_ptr<const BinaryOperationNode> node);
    static unique_ptr<Value> interpret_modulo(shared_ptr<const Value> left, shared_ptr<const Value> right, unique_ptr<const BinaryOperationNode> node);

    // helper methods:

    /// @brief Populates a value with its starting and ending position, and its context.
    /// @param value The value to populate.
    /// @param pos_start The starting position to give to the given value.
    /// @param pos_end The ending position to give to the given value.
    /// @param ctx The reference of the context to give to the given value.
    static void populate(Value& value, const Position& pos_start, const Position& pos_end, shared_ptr<const Context> ctx);

    /// @brief Populates a value with its starting and ending position, deduced from the given node, and its context.
    /// @param value The value to populate.
    /// @param node The node that created this value, and whose position must be passed to the given value.
    /// @param ctx The reference of the context to give to the given value.
    static void populate(Value& value, unique_ptr<const CustomNode> node, shared_ptr<const Context> ctx);
    
    /// @brief Throws a `RuntimeError` for an illegal operation (like "5 + a_function" for example).
    /// @param node The node that created this issue.
    /// @param ctx The context in which this issue happened.
    static void illegal_operation(unique_ptr<const CustomNode> node, shared_ptr<const Context> ctx);

    /// @brief Throws a `TypeError` for trying to assign an incompatible type to a variable.
    /// @param value The value whose type differs from the `expected_type` (or is not castable into the `expected_type`).
    /// @param expected_type The type of the variable.
    /// @param ctx The context in which the error occured.
    static void type_error(shared_ptr<const Value> value, const Type expected_type, shared_ptr<const Context> ctx);

    /// @brief Populates a value with the positions of the given node and the `shared_ctx`,
    /// and registers a successfull action in the provided RuntimeResult instance.
    /// @param res The RuntimeResult created by a visit method.
    /// @param value The value to populate with positions & context.
    /// @param node The node that the Interpreter is visiting and that produced the given value.
    static void make_success(unique_ptr<RuntimeResult>& res, unique_ptr<Value> value, unique_ptr<const CustomNode> node);

    /// @brief Applies a binary mathematical operation between `left` and `right` during the interpretation of `node`.
    /// The operation to apply is given as a lambda function via the `operation` argument.
    /// This method will populate the new value with the positions of the provided `node` and the `shared_ctx`.
    /// @tparam A The exact type of the left member.
    /// @tparam B The exact type of the right member.
    /// @tparam Op The lambda function that's automatically deduced when calling this function. No need to specify it explicitely.
    /// @param left The left member of the operation.
    /// @param right The right member of the operation.
    /// @param node The binary operation node that the Interpreter is currently interpreting.
    /// @return An instance of `Value` from the operation.
    template <typename A, typename B, typename Op>
    static unique_ptr<Value> make_operation(shared_ptr<const Value>& left, shared_ptr<const Value>& right, unique_ptr<const BinaryOperationNode> node, Op operation) {
      const A* a = dynamic_cast<const A*>(left.get());
      const B* b = dynamic_cast<const B*>(right.get());
      auto r = operation(*a, *b);
      if (r == nullptr) {
        // It's possible in some cases that during the operation
        // it fails and needs to throw a RuntimeError.
        // It happens in this example: string * int (if int is negative).
        // Therefore I delete the values, and the node should get deallocated separately.
        illegal_operation(move(node), shared_ctx);
      }
      populate(*r, move(node), shared_ctx);
      return unique_ptr<Value>(r);
    }

    /// @brief Applies an addition between `left` and `right`.
    template <typename A, typename B>
    static unique_ptr<Value> make_addition(shared_ptr<const Value>& left, shared_ptr<const Value>& right, unique_ptr<const BinaryOperationNode> node) {
      return make_operation<A, B>(left, right, move(node), [](const A& a, const B& b) { return a + b; });
    }

    /// @brief Applies a substraction between `left` and `right`.
    template <typename A, typename B>
    static unique_ptr<Value> make_substraction(shared_ptr<const Value> left, shared_ptr<const Value> right, unique_ptr<const BinaryOperationNode> node) {
      return make_operation<A, B>(left, right, move(node), [](const A& a, const B& b) { return a - b; });
    }

    /// @brief Applies a multiplication between `left` and `right`.
    template <typename A, typename B>
    static unique_ptr<Value> make_multiplication(shared_ptr<const Value> left, shared_ptr<const Value> right, unique_ptr<const BinaryOperationNode> node) {
      return make_operation<A, B>(left, right, move(node), [](const A& a, const B& b) { return a * b; });
    }

    /// @brief Applies a power operation between `left` and `right`.
    /// @tparam R Since the result type cannot be deduced, it must be specified when calling this method.
    template <typename A, typename B, typename R>
    static unique_ptr<Value> make_power(shared_ptr<const Value> left, shared_ptr<const Value> right, unique_ptr<const BinaryOperationNode> node) {
      return make_operation<A, B>(left, right, move(node), [](const A& a, const B& b) { return new R(std::pow(a.get_actual_value(), b.get_actual_value())); });
    }

    /// @brief Applies a division between `left` and `right`.
    template <typename A, typename B>
    static unique_ptr<Value> make_division(shared_ptr<const Value> left, shared_ptr<const Value> right, unique_ptr<const BinaryOperationNode> node) {
      return make_operation<A, B>(left, right, move(node), [](const A& a, const B& b) { return a / b; });
    }

    /// @brief Applies a modulo between `left` and `right`.
    template <typename A, typename B>
    static unique_ptr<Value> make_modulo(shared_ptr<const Value> left, shared_ptr<const Value> right, unique_ptr<const BinaryOperationNode> node) {
      return make_operation<A, B>(left, right, move(node), [](const A& a, const B& b) { return a % b; });
    }
};
