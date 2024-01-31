#include "../../include/nodes/string_node.hpp"
using namespace std;

StringNode::StringNode(
  const Token& token
): CustomNode(token.getStartingPosition(), token.getEndingPosition()), token(token) {}

bool StringNode::canConcatenate() const {
  return token.canConcatenate();
}

const Token* StringNode::getToken() const {
  return &token;
}

string StringNode::getValue() const {
  return token.getStringValue();
}

string StringNode::to_string() const {
  const string quote = string(1, canConcatenate() ? '"' : '\'');
  return "(" + quote + token.getStringValue() + quote + ")";
}