#include "../../include/nodes/integer_node.hpp"
using namespace std;

IntegerNode::IntegerNode(
  const Token& token
): CustomNode(token.getStartingPosition(), token.getEndingPosition()), token(token) {}

Token IntegerNode::get_token() const {
  return token;
}

string IntegerNode::to_string() const {
  return "IntegerNode(" + token.getStringValue() + ")";
}