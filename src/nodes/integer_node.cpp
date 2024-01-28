#include "../../include/nodes/integer_node.hpp"

IntegerNode::IntegerNode(
  const Token& token
): CustomNode(token.getStartingPosition(), token.getEndingPosition()), token(token) {}

const Token IntegerNode::get_token() const {
  return token;
}

string IntegerNode::to_string() const {
  return "IntegerNode(" + token.getStringValue() + ")";
}