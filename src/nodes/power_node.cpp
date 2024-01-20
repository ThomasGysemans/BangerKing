#include "../../include/nodes/power_node.hpp"

PowerNode::PowerNode(
  unique_ptr<CustomNode>& a,
  unique_ptr<CustomNode>& b
): BinaryOperationNode(move(a), move(b)) { }

string PowerNode::to_string() const {
  return "PowerNode(" + node_a->to_string() + "**" + node_b->to_string() + ")";
}