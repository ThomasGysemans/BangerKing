#include "../../include/nodes/integer_node.hpp"

IntegerNode::IntegerNode(
  const Token& token
): CustomNode(token.getStartingPosition(), token.getEndingPosition()), value(stoi(token.getStringValue())) {}

int IntegerNode::getValue() const { return value; }

string IntegerNode::to_string() const {
  return "IntegerNode(" + std::to_string(value) + ")";
}