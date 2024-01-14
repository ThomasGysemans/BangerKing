#include "../../include/nodes/double_node.hpp"
#include "../../include/utils/double_to_string.hpp"

DoubleNode::DoubleNode(
  const Token& token
):
  CustomNode(token.getStartingPosition(), token.getEndingPosition()),
  value(stod(token.getStringValue()))
{}

double DoubleNode::getValue() const { return value; }

string DoubleNode::to_string() const {
  return "DoubleNode(" + double_to_string(value) + ")";
}