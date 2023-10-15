#include "../include/miscellaneous.hpp"
#include "../include/interpreter.hpp"
#include "../include/exceptions/undefined_behavior.hpp"
#include "../include/exceptions/runtime_error.hpp"

#include <iostream>

const RuntimeResult* Interpreter::visit(const CustomNode* node, const Context& ctx) const {
  if (instanceof<NumberNode>(node)) {
    return visit_NumberNode(cast_node<NumberNode>(node), ctx);
  } else if (instanceof<AddNode>(node)) {
    cout << "Just visited an AddNode" << endl;
    // return visit_AddNode(cast_node<AddNode>(node), ctx);
  }
  throw UndefinedBehaviorException("Unimplemented visit method for input node");
}

/*
*
* Visit methods
*
*/

const RuntimeResult* Interpreter::visit_NumberNode(const NumberNode* node, const Context& ctx) const {
  IntegerValue* i = new IntegerValue(node->getValue());
  i->set_pos(node->getStartingPosition(), node->getEndingPosition());
  i->set_ctx(&ctx);
  return (new RuntimeResult())->success(dynamic_cast<Value<>*>(i));
}