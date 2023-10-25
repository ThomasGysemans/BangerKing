#include <cmath>
#include "../include/miscellaneous.hpp"
#include "../include/interpreter.hpp"
#include "../include/symbol_table.hpp"
#include "../include/exceptions/undefined_behavior.hpp"
#include "../include/exceptions/runtime_error.hpp"

RuntimeResult* Interpreter::visit(const CustomNode* node, const Context& ctx) const {
  if (instanceof<ListNode>(node)) {
    return visit_ListNode(cast_node<ListNode>(node), ctx);
  } else if (instanceof<NumberNode>(node)) {
    return visit_NumberNode(cast_node<NumberNode>(node), ctx);
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
  }
  throw UndefinedBehaviorException("Unimplemented visit method for input node");
}

/*
*
* Helper methods
*
*/

void Interpreter::populate(Value* value, const Position& pos_start, const Position& pos_end, const Context& ctx) const {
  value->set_pos(pos_start, pos_end);
  value->set_ctx(&ctx);
}

void Interpreter::populate(Value* value, const CustomNode* node, const Context& ctx) const {
  value->set_pos(node->getStartingPosition(), node->getEndingPosition());
  value->set_ctx(&ctx);
}

Value* Interpreter::to_value(Value* value) const { return dynamic_cast<Value*>(value); }

void Interpreter::illegal_operation(const CustomNode* node, const Context& ctx) const {
  Position start = node->getStartingPosition();
  Position end = node->getEndingPosition();
  delete node;
  throw RuntimeError(
    start, end,
    "Illegal operation",
    &ctx
  );
}

/*
*
* Visit methods
*
*/

RuntimeResult* Interpreter::visit_ListNode(const ListNode* node, const Context& ctx) const {
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

RuntimeResult* Interpreter::visit_NumberNode(const NumberNode* node, const Context& ctx) const {
  RuntimeResult* res = new RuntimeResult();
  IntegerValue* i = new IntegerValue(node->getValue());
  populate(i, node, ctx);
  return res->success(to_value(i));
}

RuntimeResult* Interpreter::visit_AddNode(const AddNode* node, const Context& ctx) const {
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

RuntimeResult* Interpreter::visit_SubstractNode(const SubstractNode* node, const Context& ctx) const {
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

RuntimeResult* Interpreter::visit_MultiplyNode(const MultiplyNode* node, const Context& ctx) const {
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

RuntimeResult* Interpreter::visit_PowerNode(const PowerNode* node, const Context& ctx) const {
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

RuntimeResult* Interpreter::visit_DivideNode(const DivideNode* node, const Context& ctx) const {
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

RuntimeResult* Interpreter::visit_ModuloNode(const ModuloNode* node, const Context& ctx) const {
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

RuntimeResult* Interpreter::visit_MinusNode(const MinusNode* node, const Context& ctx) const {
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

RuntimeResult* Interpreter::visit_PlusNode(const PlusNode* node, const Context& ctx) const {
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

RuntimeResult* Interpreter::visit_VarAssignmentNode(const VarAssignmentNode* node, const Context& ctx) const {
  RuntimeResult* res = new RuntimeResult();
  Value* initial_value = node->has_value() ? res->read(visit(node->get_value_node(), ctx)) : nullptr;
  if (res->should_return()) return res;

  if (ctx.get_symbol_table()->exists(node->get_var_name())) {
    throw RuntimeError(
      node->getStartingPosition(), node->getEndingPosition(),
      "The variable named '" + node->get_var_name() + "' already defined in the current context.",
      &ctx
    );
  }

  // A default value must be assigned
  // if the developer didn't set an initial value.
  // This default value will depend on the given type.
  if (!node->has_value()) {
    // TODO: if the type is a custom object, then call, if possible, the empty constructor
    switch (node->get_type()) {
      case Type::INT: initial_value = new IntegerValue(); break;
      default:
        throw RuntimeError(
          node->getStartingPosition(), node->getEndingPosition(),
          "The variable named '" + node->get_var_name() + "' cannot receive a default value for this type.",
          &ctx
        );
    }
  }

  populate(initial_value, node, ctx);
  ctx.get_symbol_table()->set(node->get_var_name(), initial_value->copy()); // copy's important because the garbage collector deallocates the returning value

  return res->success(initial_value);
}