#include "../../include/nodes/number_node.hpp"

NumberNode::NumberNode(
  const Token& token
): CustomNode(token.getStartingPosition(), token.getEndingPosition()), value(stoi(token.getStringValue())) {}

int NumberNode::getValue() const { return value; }

string NumberNode::to_string() const {
  return "NumberNode(" + std::to_string(value) + ")";
}