#include "../../include/nodes/boolean_node.hpp"
using namespace std;

BooleanNode::BooleanNode(
  const Token& token
): CustomNode(token.getStartingPosition(), token.getEndingPosition(), NodeType::BOOLEAN), token(token) {}

const Token* BooleanNode::getToken() const {
  return &token;
}

bool BooleanNode::is_true() const {
  return token.getStringValue() == "true";
}

string BooleanNode::to_string() const {
  return "(" + token.getStringValue() + ")";
}