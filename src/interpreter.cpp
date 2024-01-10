#include <cmath>
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
  throw UndefinedBehaviorException("Unimplemented visit method for input node");
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
  delete value;
  throw TypeError(
    pos_start, pos_end,
    "Type '" + get_type_name(value->get_type()) + "' is not assignable to type '" + get_type_name(expected_type) + "'",
    ctx
  );
}

// For know, it will simply compare if the types are the same,
// but it will get much more complex soon.
void Interpreter::ensure_type_compatibility(const Value* new_value, const Type expected_type, const Context* ctx) {
  const Type value_type = new_value->get_type();
  if (expected_type != value_type) {
    type_error(
      new_value,
      expected_type,
      ctx
    );
  }
}

void Interpreter::ensure_type_compatibility(const Value* new_value, const string variable_name, const Context* ctx) {
  ensure_type_compatibility(new_value, ctx->get_symbol_table()->get(variable_name)->get_type(), ctx);
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
  }

  delete value;
  delete res;
  illegal_operation(node, shared_ctx);
  return nullptr;
}

Value* Interpreter::interpret_addition(const Value* left, const Value* right, const BinaryOperationNode* node) {
  if (instanceof<IntegerValue>(left) && instanceof<IntegerValue>(right)) {
    const IntegerValue* a = dynamic_cast<const IntegerValue*>(left);
    const IntegerValue* b = dynamic_cast<const IntegerValue*>(right);
    IntegerValue* r = (*a) + (*b);
    populate(r, node, shared_ctx);
    delete a;
    delete b;
    return to_value(r);
  }

  illegal_operation(node, shared_ctx);
  return nullptr;
}

Value* Interpreter::interpret_substraction(const Value* left, const Value* right, const BinaryOperationNode* node) {
  if (instanceof<IntegerValue>(left) && instanceof<IntegerValue>(right)) {
    const IntegerValue* a = dynamic_cast<const IntegerValue*>(left);
    const IntegerValue* b = dynamic_cast<const IntegerValue*>(right);
    IntegerValue* r = (*a) - (*b);
    populate(r, node, shared_ctx);
    delete a;
    delete b;
    return to_value(r);
  }

  illegal_operation(node, shared_ctx);
  return nullptr;
}

Value* Interpreter::interpret_multiplication(const Value* left, const Value* right, const BinaryOperationNode* node) {
  if (instanceof<IntegerValue>(left) && instanceof<IntegerValue>(right)) {
    const IntegerValue* a = dynamic_cast<const IntegerValue*>(left);
    const IntegerValue* b = dynamic_cast<const IntegerValue*>(right);
    IntegerValue* r = (*a) * (*b);
    populate(r, node, shared_ctx);
    delete a;
    delete b;
    return to_value(r);
  }

  illegal_operation(node, shared_ctx);
  return nullptr;
}

Value* Interpreter::interpret_power(const Value* left, const Value* right, const BinaryOperationNode* node) {
  if (instanceof<IntegerValue>(left) && instanceof<IntegerValue>(right)) {
    const IntegerValue* a = dynamic_cast<const IntegerValue*>(left);
    const IntegerValue* b = dynamic_cast<const IntegerValue*>(right);
    IntegerValue* r = new IntegerValue(pow(a->get_actual_value(), b->get_actual_value()));
    populate(r, node, shared_ctx);
    delete a;
    delete b;
    return to_value(r);
  }

  illegal_operation(node, shared_ctx);
  return nullptr;
}

Value* Interpreter::interpret_division(const Value* left, const Value* right, const BinaryOperationNode* node) {
  if (instanceof<IntegerValue>(left) && instanceof<IntegerValue>(right)) {
    const IntegerValue* a = dynamic_cast<const IntegerValue*>(left);
    const IntegerValue* b = dynamic_cast<const IntegerValue*>(right);
    IntegerValue* r = (*a) / (*b);
    populate(r, node, shared_ctx);
    delete a;
    delete b;
    return to_value(r);
  }

  illegal_operation(node, shared_ctx);
  return nullptr;
}

Value* Interpreter::interpret_modulo(const Value* left, const Value* right, const BinaryOperationNode* node) {
  if (instanceof<IntegerValue>(left) && instanceof<IntegerValue>(right)) {
    const IntegerValue* a = dynamic_cast<const IntegerValue*>(left);
    const IntegerValue* b = dynamic_cast<const IntegerValue*>(right);
    IntegerValue* r = (*a) % (*b);
    populate(r, node, shared_ctx);
    delete a;
    delete b;
    return to_value(r);
  }

  illegal_operation(node, shared_ctx);
  return nullptr;
}

RuntimeResult* Interpreter::visit_BinaryOperationNode(const BinaryOperationNode* node) {
  RuntimeResult* res = new RuntimeResult();
  const Value* left = res->read(visit(node->get_a()));
  if (res->should_return()) return res;
  const Value* right = res->read(visit(node->get_b()));
  if (res->should_return()) return res;

  if (instanceof<AddNode>(node)) {
    return res->success(interpret_addition(left, right, node));
  } else if (instanceof<SubstractNode>(node)) {
    return res->success(interpret_substraction(left, right, node));
  } else if (instanceof<MultiplyNode>(node)) {
    return res->success(interpret_multiplication(left, right, node));
  } else if (instanceof<PowerNode>(node)) {
    return res->success(interpret_power(left, right, node));
  } else if (instanceof<DivideNode>(node)) {
    return res->success(interpret_division(left, right, node));
  } else if (instanceof<ModuloNode>(node)) {
    return res->success(interpret_modulo(left, right, node));
  }

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
      default:
        throw RuntimeError(
          node->getStartingPosition(), node->getEndingPosition(),
          "The variable named '" + node->get_var_name() + "' cannot receive a default value for this type.",
          shared_ctx
        );
    }
  } else {
    // if a default value was given, we have to make sure that its type is compatible with the given type
    // (store a as int = "hello" doesn't work).
    ensure_type_compatibility(initial_value, node->get_type(), shared_ctx);
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

  ensure_type_compatibility(new_value, node->get_var_name(), shared_ctx);

  shared_ctx->get_symbol_table()->modify(node->get_var_name(), new_value);

  // We have to keep in mind that the garbage collector will deallocate the returned value of a statement.
  // To make sure it doesn't delete a variable, we have to return a copy.
  return res->success(new_value->copy());
}