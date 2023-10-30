#include <cmath>
#include "../include/miscellaneous.hpp"
#include "../include/interpreter.hpp"
#include "../include/symbol_table.hpp"
#include "../include/exceptions/undefined_behavior.hpp"
#include "../include/exceptions/runtime_error.hpp"
#include "../include/exceptions/type_error.hpp"

RuntimeResult* Interpreter::visit(const CustomNode* node, const Context* ctx) {
  if (instanceof<ListNode>(node)) {
    return visit_ListNode(cast_node<ListNode>(node), ctx);
  } else if (instanceof<IntegerNode>(node)) {
    return visit_IntegerNode(cast_node<IntegerNode>(node), ctx);
  } else if (instanceof<AddNode>(node)) {
    return visit_AddNode(cast_node<AddNode>(node), ctx);
  } else if (instanceof<SubstractNode>(node)) {
    return visit_SubstractNode(cast_node<SubstractNode>(node), ctx);
  } else if (instanceof<MultiplyNode>(node)) {
    return visit_MultiplyNode(cast_node<MultiplyNode>(node), ctx);
  } else if (instanceof<PowerNode>(node)) {
    return visit_PowerNode(cast_node<PowerNode>(node), ctx);
  } else if (instanceof<DivideNode>(node)) {
    return visit_DivideNode(cast_node<DivideNode>(node), ctx);
  } else if (instanceof<ModuloNode>(node)) {
    return visit_ModuloNode(cast_node<ModuloNode>(node), ctx);
  } else if (instanceof<MinusNode>(node)) {
    return visit_MinusNode(cast_node<MinusNode>(node), ctx);
  } else if (instanceof<PlusNode>(node)) {
    return visit_PlusNode(cast_node<PlusNode>(node), ctx);
  } else if (instanceof<VarAssignmentNode>(node)) {
    return visit_VarAssignmentNode(cast_node<VarAssignmentNode>(node), ctx);
  } else if (instanceof<VarAccessNode>(node)) {
    return visit_VarAccessNode(cast_node<VarAccessNode>(node), ctx);
  } else if (instanceof<VarModifyNode>(node)) {
    return visit_VarModifyNode(cast_node<VarModifyNode>(node), ctx);
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

RuntimeResult* Interpreter::visit_ListNode(const ListNode* node, const Context* ctx) {
  RuntimeResult* res = new RuntimeResult();
  list<const Value*> elements;
  for (const CustomNode* element_node : node->get_element_nodes()) {
    const Value* value = res->read(visit(element_node, ctx));
    if (res->should_return()) return res;
    elements.push_back(value);
  }
  ListValue* list_value = new ListValue(elements);
  populate(list_value, node, ctx);
  return res->success(to_value(list_value));
}

RuntimeResult* Interpreter::visit_IntegerNode(const IntegerNode* node, const Context* ctx) {
  RuntimeResult* res = new RuntimeResult();
  IntegerValue* i = new IntegerValue(node->getValue());
  populate(i, node, ctx);
  return res->success(to_value(i));
}

RuntimeResult* Interpreter::visit_AddNode(const AddNode* node, const Context* ctx) {
  RuntimeResult* res = new RuntimeResult();
  const Value* left = res->read(visit(node->get_a(), ctx));
  if (res->should_return()) return res;
  const Value* right = res->read(visit(node->get_b(), ctx));
  if (res->should_return()) return res;

  if (instanceof<IntegerValue>(left) && instanceof<IntegerValue>(right)) {
    const IntegerValue* a = dynamic_cast<const IntegerValue*>(left);
    const IntegerValue* b = dynamic_cast<const IntegerValue*>(right);
    IntegerValue* r = (*a) + (*b);
    populate(r, node, ctx);
    delete a;
    delete b;
    return res->success(to_value(r));
  }

  delete left;
  delete right;
  delete res;
  illegal_operation(node, ctx);
  return nullptr;
}

RuntimeResult* Interpreter::visit_SubstractNode(const SubstractNode* node, const Context* ctx) {
  RuntimeResult* res = new RuntimeResult();
  const Value* left = res->read(visit(node->get_a(), ctx));
  if (res->should_return()) return res;
  const Value* right = res->read(visit(node->get_b(), ctx));
  if (res->should_return()) return res;

  if (instanceof<IntegerValue>(left) && instanceof<IntegerValue>(right)) {
    const IntegerValue* a = dynamic_cast<const IntegerValue*>(left);
    const IntegerValue* b = dynamic_cast<const IntegerValue*>(right);
    IntegerValue* r = (*a) - (*b);
    populate(r, node, ctx);
    delete a;
    delete b;
    return res->success(to_value(r));
  }

  delete left;
  delete right;
  delete res;
  illegal_operation(node, ctx);
  return nullptr;
}

RuntimeResult* Interpreter::visit_MultiplyNode(const MultiplyNode* node, const Context* ctx) {
  RuntimeResult* res = new RuntimeResult();
  const Value* left = res->read(visit(node->get_a(), ctx));
  if (res->should_return()) return res;
  const Value* right = res->read(visit(node->get_b(), ctx));
  if (res->should_return()) return res;

  if (instanceof<IntegerValue>(left) && instanceof<IntegerValue>(right)) {
    const IntegerValue* a = dynamic_cast<const IntegerValue*>(left);
    const IntegerValue* b = dynamic_cast<const IntegerValue*>(right);
    IntegerValue* r = (*a) * (*b);
    populate(r, node, ctx);
    delete a;
    delete b;
    return res->success(to_value(r));
  }

  delete left;
  delete right;
  delete res;
  illegal_operation(node, ctx);
  return nullptr;
}

RuntimeResult* Interpreter::visit_PowerNode(const PowerNode* node, const Context* ctx) {
  RuntimeResult* res = new RuntimeResult();
  const Value* left = res->read(visit(node->get_a(), ctx));
  if (res->should_return()) return res;
  const Value* right = res->read(visit(node->get_b(), ctx));
  if (res->should_return()) return res;

  if (instanceof<IntegerValue>(left) && instanceof<IntegerValue>(right)) {
    const IntegerValue* a = dynamic_cast<const IntegerValue*>(left);
    const IntegerValue* b = dynamic_cast<const IntegerValue*>(right);
    IntegerValue* r = new IntegerValue(pow(a->get_actual_value(), b->get_actual_value()));
    populate(r, node, ctx);
    delete a;
    delete b;
    return res->success(to_value(r));
  }

  delete left;
  delete right;
  delete res;
  illegal_operation(node, ctx);
  return nullptr;
}

RuntimeResult* Interpreter::visit_DivideNode(const DivideNode* node, const Context* ctx) {
  RuntimeResult* res = new RuntimeResult();
  const Value* left = res->read(visit(node->get_a(), ctx));
  if (res->should_return()) return res;
  const Value* right = res->read(visit(node->get_b(), ctx));
  if (res->should_return()) return res;

  if (instanceof<IntegerValue>(left) && instanceof<IntegerValue>(right)) {
    const IntegerValue* a = dynamic_cast<const IntegerValue*>(left);
    const IntegerValue* b = dynamic_cast<const IntegerValue*>(right);
    IntegerValue* r = (*a) / (*b);
    populate(r, node, ctx);
    delete a;
    delete b;
    return res->success(to_value(r));
  }

  delete left;
  delete right;
  delete res;
  illegal_operation(node, ctx);
  return nullptr;
}

RuntimeResult* Interpreter::visit_ModuloNode(const ModuloNode* node, const Context* ctx) {
  RuntimeResult* res = new RuntimeResult();
  const Value* left = res->read(visit(node->get_a(), ctx));
  if (res->should_return()) return res;
  const Value* right = res->read(visit(node->get_b(), ctx));
  if (res->should_return()) return res;

  if (instanceof<IntegerValue>(left) && instanceof<IntegerValue>(right)) {
    const IntegerValue* a = dynamic_cast<const IntegerValue*>(left);
    const IntegerValue* b = dynamic_cast<const IntegerValue*>(right);
    IntegerValue* r = (*a) % (*b);
    populate(r, node, ctx);
    delete a;
    delete b;
    return res->success(to_value(r));
  }

  delete left;
  delete right;
  delete res;
  illegal_operation(node, ctx);
  return nullptr;
}

RuntimeResult* Interpreter::visit_MinusNode(const MinusNode* node, const Context* ctx) {
  RuntimeResult* res = new RuntimeResult();
  const Value* value = res->read(visit(node->get_node(), ctx));
  if (res->should_return()) return res;

  if (instanceof<IntegerValue>(value)) {
    const IntegerValue* integer = dynamic_cast<const IntegerValue*>(value);
    IntegerValue* negative_integer = new IntegerValue(-1 * integer->get_actual_value());
    populate(negative_integer, node, ctx);
    delete value;
    return res->success(to_value(negative_integer));
  }

  delete value;
  delete res;
  illegal_operation(node, ctx);
  return nullptr;
}

RuntimeResult* Interpreter::visit_PlusNode(const PlusNode* node, const Context* ctx) {
  RuntimeResult* res = new RuntimeResult();
  const Value* value = res->read(visit(node->get_node(), ctx));
  if (res->should_return()) return res;

  if (instanceof<IntegerValue>(value)) {
    const IntegerValue* integer = dynamic_cast<const IntegerValue*>(value);
    IntegerValue* positive_integer = new IntegerValue(abs(integer->get_actual_value()));
    populate(positive_integer, node, ctx);
    delete value;
    return res->success(to_value(positive_integer));
  }

  delete value;
  delete res;
  illegal_operation(node, ctx);
  return nullptr;
}

RuntimeResult* Interpreter::visit_VarAssignmentNode(const VarAssignmentNode* node, const Context* ctx) {
  if (ctx->get_symbol_table()->exists(node->get_var_name())) {
    throw RuntimeError(
      node->getStartingPosition(), node->getEndingPosition(),
      "The variable named '" + node->get_var_name() + "' already defined in the current context.",
      ctx
    );
  }

  RuntimeResult* res = new RuntimeResult();
  Value* initial_value = node->has_value() ? res->read(visit(node->get_value_node(), ctx)) : nullptr;
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
          ctx
        );
    }
  } else {
    // if a default value was given, we have to make sure that its type is compatible with the given type
    // (store a as int = "hello" doesn't work).
    ensure_type_compatibility(initial_value, node->get_type(), ctx);
  }

  populate(initial_value, node, ctx);
  ctx->get_symbol_table()->set(node->get_var_name(), initial_value->copy()); // copy's important because the garbage collector deallocates the returning value

  return res->success(initial_value);
}

RuntimeResult* Interpreter::visit_VarAccessNode(const VarAccessNode* node, const Context* ctx) {
  if (!ctx->get_symbol_table()->exists_globally(node->get_var_name())) {
    throw RuntimeError(
      node->getStartingPosition(), node->getEndingPosition(),
      "Undefined variable '" + node->get_var_name() + "'.",
      ctx
    );
  }
  
  RuntimeResult* res = new RuntimeResult();
  Value* value = ctx->get_symbol_table()->get(node->get_var_name());
  populate(value, node, ctx);

  // We have to keep in mind that the garbage collector will deallocate the returned value of a statement.
  // To make sure it doesn't delete a variable, we have to return a copy.
  return res->success(value->copy());
}

RuntimeResult* Interpreter::visit_VarModifyNode(const VarModifyNode* node, const Context* ctx) {
  if (!ctx->get_symbol_table()->exists_globally(node->get_var_name())) {
    throw RuntimeError(
      node->getStartingPosition(), node->getEndingPosition(),
      "Undefined variable '" + node->get_var_name() + "'.",
      ctx
    );
  }
  
  RuntimeResult* res = new RuntimeResult();
  Value* new_value = res->read(visit(node->get_value_node(), ctx));
  if (res->should_return()) return res;

  ensure_type_compatibility(new_value, node->get_var_name(), ctx);

  ctx->get_symbol_table()->modify(node->get_var_name(), new_value);

  // We have to keep in mind that the garbage collector will deallocate the returned value of a statement.
  // To make sure it doesn't delete a variable, we have to return a copy.
  return res->success(new_value->copy());
}