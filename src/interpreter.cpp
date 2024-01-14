#include "../include/miscellaneous.hpp"
#include "../include/interpreter.hpp"
#include "../include/symbol_table.hpp"
#include "../include/exceptions/undefined_behavior.hpp"
#include "../include/exceptions/runtime_error.hpp"
#include "../include/exceptions/type_error.hpp"

// Since `shared_ctx` is static,
// it must be redeclared here so that the compiler knows it exists.
Context* Interpreter::shared_ctx = nullptr;

void Interpreter::set_shared_ctx(Context* ctx) {
  shared_ctx = ctx;
}

RuntimeResult* Interpreter::visit(const CustomNode* node) {
  if (shared_ctx == nullptr) {
    throw "A context was not provided for interpretation.";
  }
  if (instanceof<ListNode>(node)) {
    return visit_ListNode(cast_node<ListNode>(node));
  } else if (instanceof<IntegerNode>(node)) {
    return visit_IntegerNode(cast_node<IntegerNode>(node));
  } else if (instanceof<DoubleNode>(node)) {
    return visit_DoubleNode(cast_node<DoubleNode>(node));
  } else if (instanceof<BinaryOperationNode>(node)) {
    return visit_BinaryOperationNode(cast_node<BinaryOperationNode>(node));
  } else if (instanceof<MinusNode>(node)) {
    return visit_MinusNode(cast_node<MinusNode>(node));
  } else if (instanceof<PlusNode>(node)) {
    return visit_PlusNode(cast_node<PlusNode>(node));
  } else if (instanceof<VarAssignmentNode>(node)) {
    return visit_VarAssignmentNode(cast_node<VarAssignmentNode>(node));
  } else if (instanceof<VarAccessNode>(node)) {
    return visit_VarAccessNode(cast_node<VarAccessNode>(node));
  } else if (instanceof<VarModifyNode>(node)) {
    return visit_VarModifyNode(cast_node<VarModifyNode>(node));
  }
  throw UndefinedBehaviorException("Unimplemented visit method for input node '" + node->to_string() + "'");
}

/*
*
* Helper methods
*
*/

void Interpreter::populate(Value* value, const Position& pos_start, const Position& pos_end, const Context* ctx) {
  value->set_pos(pos_start, pos_end);
  value->set_ctx(ctx);
}

void Interpreter::populate(Value* value, const CustomNode* node, const Context* ctx) {
  value->set_pos(node->getStartingPosition(), node->getEndingPosition());
  value->set_ctx(ctx);
}

Value* Interpreter::to_value(Value* value) { return dynamic_cast<Value*>(value); }

void Interpreter::illegal_operation(const CustomNode* node, const Context* ctx) {
  const Position start = node->getStartingPosition();
  const Position end = node->getEndingPosition();
  delete node;
  throw RuntimeError(
    start, end,
    "Illegal operation",
    ctx
  );
}

void Interpreter::type_error(const Value* value, const Type expected_type, const Context* ctx) {
  const Position pos_start = *(value->get_pos_start());
  const Position pos_end = *(value->get_pos_end());
  const Type type = value->get_type();
  delete value;
  throw TypeError(
    pos_start, pos_end,
    "Type '" + get_type_name(type) + "' is not assignable to type '" + get_type_name(expected_type) + "'",
    ctx
  );
}

/*
*
* Visit methods
*
*/

RuntimeResult* Interpreter::visit_ListNode(const ListNode* node) {
  RuntimeResult* res = new RuntimeResult();
  list<const Value*> elements;
  for (const CustomNode* element_node : node->get_element_nodes()) {
    const Value* value = res->read(visit(element_node));
    if (res->should_return()) return res;
    elements.push_back(value);
  }
  ListValue* list_value = new ListValue(elements);
  populate(list_value, node, shared_ctx);
  return res->success(to_value(list_value));
}

RuntimeResult* Interpreter::visit_IntegerNode(const IntegerNode* node) {
  RuntimeResult* res = new RuntimeResult();
  IntegerValue* i = new IntegerValue(node->getValue());
  populate(i, node, shared_ctx);
  return res->success(to_value(i));
}

RuntimeResult* Interpreter::visit_DoubleNode(const DoubleNode* node) {
  RuntimeResult* res = new RuntimeResult();
  DoubleValue* i = new DoubleValue(node->getValue());
  populate(i, node, shared_ctx);
  return res->success(to_value(i));
}

RuntimeResult* Interpreter::visit_MinusNode(const MinusNode* node) {
  RuntimeResult* res = new RuntimeResult();
  const Value* value = res->read(visit(node->get_node()));
  if (res->should_return()) return res;

  if (instanceof<IntegerValue>(value)) {
    const IntegerValue* integer = dynamic_cast<const IntegerValue*>(value);
    IntegerValue* negative_integer = new IntegerValue(-1 * integer->get_actual_value());
    populate(negative_integer, node, shared_ctx);
    delete value;
    return res->success(to_value(negative_integer));
  } else if (instanceof<DoubleValue>(value)) {
    const DoubleValue* d = dynamic_cast<const DoubleValue*>(value);
    DoubleValue* negative_double = new DoubleValue(-1 * d->get_actual_value());
    populate(negative_double, node, shared_ctx);
    delete value;
    return res->success(to_value(negative_double));
  }

  delete value;
  delete res;
  illegal_operation(node, shared_ctx);
  return nullptr;
}

RuntimeResult* Interpreter::visit_PlusNode(const PlusNode* node) {
  RuntimeResult* res = new RuntimeResult();
  const Value* value = res->read(visit(node->get_node()));
  if (res->should_return()) return res;

  if (instanceof<IntegerValue>(value)) {
    const IntegerValue* integer = dynamic_cast<const IntegerValue*>(value);
    IntegerValue* positive_integer = new IntegerValue(abs(integer->get_actual_value()));
    populate(positive_integer, node, shared_ctx);
    delete value;
    return res->success(to_value(positive_integer));
  } else if (instanceof<DoubleValue>(value)) {
    const DoubleValue* d = dynamic_cast<const DoubleValue*>(value);
    DoubleValue* positive_double = new DoubleValue(abs(d->get_actual_value()));
    populate(positive_double, node, shared_ctx);
    delete value;
    return res->success(to_value(positive_double));
  }

  delete value;
  delete res;
  illegal_operation(node, shared_ctx);
  return nullptr;
}

Value* Interpreter::interpret_addition(const Value* left, const Value* right, const BinaryOperationNode* node) {
  // The permutations:
  // - int + int = int
  // - int + double = double
  // - double + double = double
  // - double + int = double
  if      (instanceof<IntegerValue>(left) && instanceof<IntegerValue>(right)) return make_addition<IntegerValue, IntegerValue>(left, right, node);
  else if (instanceof<IntegerValue>(left) && instanceof<DoubleValue>(right))  return make_addition<IntegerValue, DoubleValue>(left, right, node);
  else if (instanceof<DoubleValue>(left)  && instanceof<DoubleValue>(right))  return make_addition<DoubleValue, DoubleValue>(left, right, node);
  else if (instanceof<DoubleValue>(left)  && instanceof<IntegerValue>(right)) return make_addition<DoubleValue, IntegerValue>(left, right, node);

  illegal_operation(node, shared_ctx);
  return nullptr;
}

Value* Interpreter::interpret_substraction(const Value* left, const Value* right, const BinaryOperationNode* node) {
  // The permutations:
  // - int - int = int
  // - int - double = double
  // - double - double = double
  // - double - int = double
  if      (instanceof<IntegerValue>(left) && instanceof<IntegerValue>(right)) return make_substraction<IntegerValue, IntegerValue>(left, right, node);
  else if (instanceof<IntegerValue>(left) && instanceof<DoubleValue>(right))  return make_substraction<IntegerValue, DoubleValue>(left, right, node);
  else if (instanceof<DoubleValue>(left)  && instanceof<DoubleValue>(right))  return make_substraction<DoubleValue, DoubleValue>(left, right, node);
  else if (instanceof<DoubleValue>(left)  && instanceof<IntegerValue>(right)) return make_substraction<DoubleValue, IntegerValue>(left, right, node);
  
  illegal_operation(node, shared_ctx);
  return nullptr;
}

Value* Interpreter::interpret_multiplication(const Value* left, const Value* right, const BinaryOperationNode* node) {
  // The permutations:
  // - int * int = int
  // - int * double = double
  // - double * double = double
  // - double * int = double
  if      (instanceof<IntegerValue>(left) && instanceof<IntegerValue>(right)) return make_multiplication<IntegerValue, IntegerValue>(left, right, node);
  else if (instanceof<IntegerValue>(left) && instanceof<DoubleValue>(right))  return make_multiplication<IntegerValue, DoubleValue>(left, right, node);
  else if (instanceof<DoubleValue>(left)  && instanceof<DoubleValue>(right))  return make_multiplication<DoubleValue, DoubleValue>(left, right, node);
  else if (instanceof<DoubleValue>(left)  && instanceof<IntegerValue>(right)) return make_multiplication<DoubleValue, IntegerValue>(left, right, node);

  illegal_operation(node, shared_ctx);
  return nullptr;
}

Value* Interpreter::interpret_power(const Value* left, const Value* right, const BinaryOperationNode* node) {
  // The permutations:
  // - int ** int = int
  // - int ** double = double
  // - double ** double = double
  // - double ** int = double
  if      (instanceof<IntegerValue>(left) && instanceof<IntegerValue>(right)) return make_power<IntegerValue, IntegerValue, IntegerValue>(left, right, node);
  else if (instanceof<IntegerValue>(left) && instanceof<DoubleValue>(right))  return make_power<IntegerValue, DoubleValue, DoubleValue>(left, right, node);
  else if (instanceof<DoubleValue>(left)  && instanceof<DoubleValue>(right))  return make_power<DoubleValue, DoubleValue, DoubleValue>(left, right, node);
  else if (instanceof<DoubleValue>(left)  && instanceof<IntegerValue>(right)) return make_power<DoubleValue, IntegerValue, DoubleValue>(left, right, node);

  illegal_operation(node, shared_ctx);
  return nullptr;
}

Value* Interpreter::interpret_division(const Value* left, const Value* right, const BinaryOperationNode* node) {
  // The permutations:
  // - int / int = int
  // - int / double = double
  // - double / double = double
  // - double / int = double
  if      (instanceof<IntegerValue>(left) && instanceof<IntegerValue>(right)) return make_division<IntegerValue, IntegerValue>(left, right, node);
  else if (instanceof<IntegerValue>(left) && instanceof<DoubleValue>(right))  return make_division<IntegerValue, DoubleValue>(left, right, node);
  else if (instanceof<DoubleValue>(left)  && instanceof<DoubleValue>(right))  return make_division<DoubleValue, DoubleValue>(left, right, node);
  else if (instanceof<DoubleValue>(left)  && instanceof<IntegerValue>(right)) return make_division<DoubleValue, IntegerValue>(left, right, node);

  illegal_operation(node, shared_ctx);
  return nullptr;
}

Value* Interpreter::interpret_modulo(const Value* left, const Value* right, const BinaryOperationNode* node) {
  // The permutations:
  // - int % int = int
  // - int % double = double
  // - double % double = double
  // - double % int = double
  if      (instanceof<IntegerValue>(left) && instanceof<IntegerValue>(right)) return make_modulo<IntegerValue, IntegerValue>(left, right, node);
  else if (instanceof<IntegerValue>(left) && instanceof<DoubleValue>(right))  return make_modulo<IntegerValue, DoubleValue>(left, right, node);
  else if (instanceof<DoubleValue>(left)  && instanceof<DoubleValue>(right))  return make_modulo<DoubleValue, DoubleValue>(left, right, node);
  else if (instanceof<DoubleValue>(left)  && instanceof<IntegerValue>(right)) return make_modulo<DoubleValue, IntegerValue>(left, right, node);

  illegal_operation(node, shared_ctx);
  return nullptr;
}

RuntimeResult* Interpreter::visit_BinaryOperationNode(const BinaryOperationNode* node) {
  RuntimeResult* res = new RuntimeResult();
  const Value* left = res->read(visit(node->get_a()));
  if (res->should_return()) return res;
  const Value* right = res->read(visit(node->get_b()));
  if (res->should_return()) return res;

  if      (instanceof<AddNode>(node))       return res->success(interpret_addition(left, right, node));
  else if (instanceof<SubstractNode>(node)) return res->success(interpret_substraction(left, right, node));
  else if (instanceof<MultiplyNode>(node))  return res->success(interpret_multiplication(left, right, node));
  else if (instanceof<PowerNode>(node))     return res->success(interpret_power(left, right, node));
  else if (instanceof<DivideNode>(node))    return res->success(interpret_division(left, right, node));
  else if (instanceof<ModuloNode>(node))    return res->success(interpret_modulo(left, right, node));

  delete left;
  delete right;
  delete res;
  illegal_operation(node, shared_ctx);
  return nullptr;
}

RuntimeResult* Interpreter::visit_VarAssignmentNode(const VarAssignmentNode* node) {
  if (shared_ctx->get_symbol_table()->exists(node->get_var_name())) {
    throw RuntimeError(
      node->getStartingPosition(), node->getEndingPosition(),
      "The variable named '" + node->get_var_name() + "' already defined in the current context.",
      shared_ctx
    );
  }

  RuntimeResult* res = new RuntimeResult();
  Value* initial_value = node->has_value() ? res->read(visit(node->get_value_node())) : nullptr;
  if (res->should_return()) return res;

  // A default value must be assigned
  // if the developer didn't set an initial value.
  // This default value will depend on the given type.
  if (!node->has_value()) {
    switch (node->get_type()) {
      case Type::INT: initial_value = new IntegerValue(); break;
      case Type::DOUBLE: initial_value = new DoubleValue(); break;
      default:
        throw RuntimeError(
          node->getStartingPosition(), node->getEndingPosition(),
          "The variable named '" + node->get_var_name() + "' cannot receive a default value for this type.",
          shared_ctx
        );
    }
  } else {
    if (node->get_type() != initial_value->get_type()) {
      Value* cast_value = initial_value->cast(node->get_type());
      if (cast_value == nullptr) {
        type_error(
          initial_value,
          node->get_type(),
          shared_ctx
        );
      }
      delete initial_value;
      initial_value = cast_value;
    }
  }

  populate(initial_value, node, shared_ctx);
  shared_ctx->get_symbol_table()->set(node->get_var_name(), initial_value->copy()); // copy's important because the garbage collector deallocates the returning value

  return res->success(initial_value);
}

RuntimeResult* Interpreter::visit_VarAccessNode(const VarAccessNode* node) {
  if (!shared_ctx->get_symbol_table()->exists_globally(node->get_var_name())) {
    throw RuntimeError(
      node->getStartingPosition(), node->getEndingPosition(),
      "Undefined variable '" + node->get_var_name() + "'.",
      shared_ctx
    );
  }
  
  RuntimeResult* res = new RuntimeResult();
  Value* value = shared_ctx->get_symbol_table()->get(node->get_var_name());
  populate(value, node, shared_ctx);

  // We have to keep in mind that the garbage collector will deallocate the returned value of a statement.
  // To make sure it doesn't delete a variable, we have to return a copy.
  return res->success(value->copy());
}

RuntimeResult* Interpreter::visit_VarModifyNode(const VarModifyNode* node) {
  if (!shared_ctx->get_symbol_table()->exists_globally(node->get_var_name())) {
    throw RuntimeError(
      node->getStartingPosition(), node->getEndingPosition(),
      "Undefined variable '" + node->get_var_name() + "'.",
      shared_ctx
    );
  }
  
  RuntimeResult* res = new RuntimeResult();
  Value* new_value = res->read(visit(node->get_value_node()));
  if (res->should_return()) return res;

  // If the type isn't exactly the same,
  // then try to cast the given value
  // so as to match the one of the variable.
  // If it doesn't work, throw a TypeError.
  Value* existing_value = shared_ctx->get_symbol_table()->get(node->get_var_name());
  if (new_value->get_type() != existing_value->get_type()) {
    Value* cast_value = new_value->cast(existing_value->get_type());
    if (cast_value == nullptr) {
      type_error(
        new_value,
        existing_value->get_type(),
        shared_ctx
      );
    }
    delete new_value;
    new_value = cast_value;
  }

  shared_ctx->get_symbol_table()->modify(node->get_var_name(), new_value);

  // We have to keep in mind that the garbage collector will deallocate the returned value of a statement.
  // To make sure it doesn't delete a variable, we have to return a copy.
  return res->success(new_value->copy());
}