#include "../../include/nodes/double_node.hpp"

DoubleNode::DoubleNode(
  const Token& token
): CustomNode(token.getStartingPosition(), token.getEndingPosition()), token(token) {}

const Token DoubleNode::get_token() const {
  return token;
}

string DoubleNode::to_string() const {
  return "DoubleNode(" + token.getStringValue() + ")";
}