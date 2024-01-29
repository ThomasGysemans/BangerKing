#pragma once

#include <cmath>
#include "runtime.hpp"
#include "context.hpp"
#include "nodes/compositer.hpp"
#include "values/compositer.hpp"
#include "exceptions/arithmetic_error.hpp"

class Interpreter {
  static std::shared_ptr<Context> shared_ctx;
  
  public:
    static void set_shared_ctx(std::shared_ptr<Context> ctx);

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
    static std::unique_ptr<RuntimeResult> visit(std::unique_ptr<CustomNode> node);
    
  private:
    // Here the specific visit methods.
    // Each of them will take care of interpreting a specific node.
    // They also become the owners of the node they're interpreting,
    // so as to deallocate the memory used by the Parser progressively.
    // They become the owners because the parameter is a std::unique_ptr.

    static std::unique_ptr<RuntimeResult> visit_IntegerNode(std::unique_ptr<const IntegerNode>);
    static std::unique_ptr<RuntimeResult> visit_DoubleNode(std::unique_ptr<const DoubleNode>);
    static std::unique_ptr<RuntimeResult> visit_ListNode(std::unique_ptr<ListNode>);
    static std::unique_ptr<RuntimeResult> visit_MinusNode(std::unique_ptr<MinusNode>);
    static std::unique_ptr<RuntimeResult> visit_PlusNode(std::unique_ptr<PlusNode>);
    static std::unique_ptr<RuntimeResult> visit_VarAssignmentNode(std::unique_ptr<VarAssignmentNode>);
    static std::unique_ptr<RuntimeResult> visit_DefineConstantNode(std::unique_ptr<DefineConstantNode>);
    static std::unique_ptr<RuntimeResult> visit_VarAccessNode(std::unique_ptr<VarAccessNode>);
    static std::unique_ptr<RuntimeResult> visit_VarModifyNode(std::unique_ptr<VarModifyNode>);
    static std::unique_ptr<RuntimeResult> visit_StringNode(std::unique_ptr<const StringNode>);
    static std::unique_ptr<RuntimeResult> visit_BooleanNode(std::unique_ptr<const BooleanNode>);
    static std::unique_ptr<RuntimeResult> visit_OrNode(std::unique_ptr<OrNode>);
    static std::unique_ptr<RuntimeResult> visit_AndNode(std::unique_ptr<AndNode>);
    static std::unique_ptr<RuntimeResult> visit_NotNode(std::unique_ptr<NotNode>);

    /// @brief Explores a binary operation node (addition, substraction, division, power, multiplication, modulo, etc.)
    /// @param node A binary operation node.
    /// @return The intepretation of this operation as a RuntimeResult.
    static std::unique_ptr<RuntimeResult> visit_BinaryOperationNode(std::unique_ptr<BinaryOperationNode> node);

    static std::unique_ptr<Value> interpret_addition(std::shared_ptr<const Value>, std::shared_ptr<const Value>, std::unique_ptr<const BinaryOperationNode>);
    static std::unique_ptr<Value> interpret_substraction(std::shared_ptr<const Value>, std::shared_ptr<const Value>, std::unique_ptr<const BinaryOperationNode>);
    static std::unique_ptr<Value> interpret_multiplication(std::shared_ptr<const Value>, std::shared_ptr<const Value>, std::unique_ptr<const BinaryOperationNode>);
    static std::unique_ptr<Value> interpret_power(std::shared_ptr<const Value>, std::shared_ptr<const Value>, std::unique_ptr<const BinaryOperationNode>);
    static std::unique_ptr<Value> interpret_division(std::shared_ptr<const Value>, std::shared_ptr<const Value>, std::unique_ptr<const BinaryOperationNode>);
    static std::unique_ptr<Value> interpret_modulo(std::shared_ptr<const Value>, std::shared_ptr<const Value>, std::unique_ptr<const BinaryOperationNode>);

    // helper methods:

    /// @brief Populates a value with its starting and ending position, and its context.
    /// @param value The value to populate.
    /// @param pos_start The starting position to give to the given value.
    /// @param pos_end The ending position to give to the given value.
    /// @param ctx The reference of the context to give to the given value.
    static void populate(Value& value, const Position& pos_start, const Position& pos_end, std::shared_ptr<const Context> ctx);

    /// @brief Populates a value with its starting and ending position, deduced from the given node, and its context.
    /// @param value The value to populate.
    /// @param node The node that created this value, and whose position must be passed to the given value.
    /// @param ctx The reference of the context to give to the given value.
    static void populate(Value& value, std::unique_ptr<const CustomNode> node, std::shared_ptr<const Context> ctx);
    
    /// @brief Throws a `RuntimeError` for an illegal operation (like "5 + a_function" for example).
    /// @param node The node that created this issue.
    /// @param ctx The context in which this issue happened.
    static void illegal_operation(std::unique_ptr<const CustomNode> node, std::shared_ptr<const Context> ctx);

    /// @brief Throws a `TypeError` for trying to assign an incompatible type to a variable.
    /// @param value The value whose type differs from the `expected_type` (or is not castable into the `expected_type`).
    /// @param expected_type The type of the variable.
    /// @param ctx The context in which the error occured.
    static void type_error(std::shared_ptr<const Value> value, const Type expected_type, std::shared_ptr<const Context> ctx);

    /// @brief Populates a value with the positions of the given node and the `shared_ctx`,
    /// and registers a successfull action in the provided RuntimeResult instance.
    /// @param res The RuntimeResult created by a visit method.
    /// @param value The value to populate with positions & context.
    /// @param node The node that the Interpreter is visiting and that produced the given value.
    static void make_success(std::unique_ptr<RuntimeResult>& res, std::unique_ptr<Value> value, std::unique_ptr<const CustomNode> node);

    /// @brief Applies a binary mathematical operation between `left` and `right` during the interpretation of `node`.
    /// The operation to apply is given as a lambda function via the `operation` argument.
    /// This method will populate the new value with the positions of the provided `node` and the `shared_ctx`.
    /// @tparam A The exact type of the left member.
    /// @tparam B The exact type of the right member.
    /// @tparam Op The lambda function that's automatically deduced when calling this function. No need to specify it explicitely.
    /// @param left The left member of the operation.
    /// @param right The right member of the operation.
    /// @param node The binary operation node that the Interpreter is currently interpreting.
    /// @param is_division_or_modulo If the operation is a division or a modulo and an error occured during the operation, maybe it's a divison-by-zero error (ArithmeticError).
    /// @return An instance of `Value` from the operation.
    template <typename A, typename B, typename Op>
    static std::unique_ptr<Value> make_operation(std::shared_ptr<const Value>& left, std::shared_ptr<const Value>& right, std::unique_ptr<const BinaryOperationNode> node, Op operation, bool is_division_or_modulo = false) {
      const A* a = dynamic_cast<const A*>(left.get());
      const B* b = dynamic_cast<const B*>(right.get());
      auto r = operation(*a, *b);
      if (r == nullptr) {
        // It's possible in some cases that during the operation
        // it fails and needs to throw a RuntimeError.
        // It happens in this example: string * int (if int is negative).
        if (is_division_or_modulo) {
          if (
            (instanceof<IntegerValue>(right) && cast_const_value<IntegerValue>(right)->get_actual_value() == 0) ||
            (instanceof<DoubleValue>(right) && cast_const_value<DoubleValue>(right)->get_actual_value() == 0.0)
          ) {
            throw ArithmeticError(
              node->getStartingPosition(), node->getEndingPosition(),
              "Division by zero isn't possible",
              shared_ctx
            );
          }
        }
        illegal_operation(move(node), shared_ctx);
      }
      populate(*r, move(node), shared_ctx);
      return std::unique_ptr<Value>(r);
    }

    /// @brief Applies an addition between `left` and `right`.
    template <typename A, typename B>
    static std::unique_ptr<Value> make_addition(std::shared_ptr<const Value>& left, std::shared_ptr<const Value>& right, std::unique_ptr<const BinaryOperationNode> node) {
      return make_operation<A, B>(left, right, move(node), [](const A& a, const B& b) { return a + b; });
    }

    /// @brief Applies a substraction between `left` and `right`.
    template <typename A, typename B>
    static std::unique_ptr<Value> make_substraction(std::shared_ptr<const Value> left, std::shared_ptr<const Value> right, std::unique_ptr<const BinaryOperationNode> node) {
      return make_operation<A, B>(left, right, move(node), [](const A& a, const B& b) { return a - b; });
    }

    /// @brief Applies a multiplication between `left` and `right`.
    template <typename A, typename B>
    static std::unique_ptr<Value> make_multiplication(std::shared_ptr<const Value> left, std::shared_ptr<const Value> right, std::unique_ptr<const BinaryOperationNode> node) {
      return make_operation<A, B>(left, right, move(node), [](const A& a, const B& b) { return a * b; });
    }

    /// @brief Applies a power operation between `left` and `right`.
    /// @tparam R Since the result type cannot be deduced, it must be specified when calling this method.
    template <typename A, typename B, typename R>
    static std::unique_ptr<Value> make_power(std::shared_ptr<const Value> left, std::shared_ptr<const Value> right, std::unique_ptr<const BinaryOperationNode> node) {
      return make_operation<A, B>(left, right, move(node), [](const A& a, const B& b) { return new R(std::pow(a.get_actual_value(), b.get_actual_value())); });
    }

    /// @brief Applies a division between `left` and `right`.
    template <typename A, typename B>
    static std::unique_ptr<Value> make_division(std::shared_ptr<const Value> left, std::shared_ptr<const Value> right, std::unique_ptr<const BinaryOperationNode> node) {
      return make_operation<A, B>(left, right, move(node), [](const A& a, const B& b) { return a / b; }, true);
    }

    /// @brief Applies a modulo between `left` and `right`.
    template <typename A, typename B>
    static std::unique_ptr<Value> make_modulo(std::shared_ptr<const Value> left, std::shared_ptr<const Value> right, std::unique_ptr<const BinaryOperationNode> node) {
      return make_operation<A, B>(left, right, move(node), [](const A& a, const B& b) { return a % b; }, true);
    }
};
