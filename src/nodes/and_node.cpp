#include "../../include/nodes/and_node.hpp"

AndNode::AndNode(
  unique_ptr<CustomNode>& a,
  unique_ptr<CustomNode>& b
): BinaryOperationNode(move(a), move(b)) { }

string AndNode::to_string() const {
  return "AndNode(" + node_a->to_string() + " and " + node_b->to_string() + ")";
}