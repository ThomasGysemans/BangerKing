#include "../include/miscellaneous.hpp"
#include "../include/interpreter.hpp"
#include "../include/exceptions/undefined_behavior.hpp"
#include "../include/exceptions/runtime_error.hpp"

RuntimeResult* Interpreter::visit(const CustomNode* node, const Context& ctx) const {
  if (instanceof<ListNode>(node)) {
    return visit_ListNode(cast_node<ListNode>(node), ctx);
  } else if (instanceof<NumberNode>(node)) {
    return visit_NumberNode(cast_node<NumberNode>(node), ctx);
  } else if (instanceof<AddNode>(node)) {
    return visit_AddNode(cast_node<AddNode>(node), ctx);
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
