#include "../../include/nodes/double_node.hpp"
using namespace std;

DoubleNode::DoubleNode(
  const Token& token
): CustomNode(token.getStartingPosition(), token.getEndingPosition(), NodeType::DOUBLE), token(token) {}

Token DoubleNode::get_token() const {
  return token;
}

string DoubleNode::to_string() const {
  return "DoubleNode(" + token.getStringValue() + ")";
}

string DoubleNode::literal() const {
  return token.getStringValue();
}