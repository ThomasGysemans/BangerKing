#include "../../include/nodes/minus_node.hpp"
using namespace std;

MinusNode::MinusNode(
  unique_ptr<CustomNode> n
): CustomNode(n->getStartingPosition(), n->getEndingPosition(), NodeType::NEGATIVE), node(move(n)) {}

unique_ptr<CustomNode> MinusNode::retrieve_node() { return move(node); }

string MinusNode::to_string() const {
  return "(-" + node->to_string() + ")";
}