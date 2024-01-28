#include "../../include/nodes/or_node.hpp"

OrNode::OrNode(
  unique_ptr<CustomNode>& a,
  unique_ptr<CustomNode>& b
): BinaryOperationNode(move(a), move(b)) { }

string OrNode::to_string() const {
  return "OrNode(" + node_a->to_string() + " or " + node_b->to_string() + ")";
}