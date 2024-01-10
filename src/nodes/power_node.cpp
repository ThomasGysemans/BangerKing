#include "../../include/nodes/power_node.hpp"

PowerNode::PowerNode(
  const CustomNode* a,
  const CustomNode* b
): BinaryOperationNode(a, b) { }

string PowerNode::to_string() const {
  return "PowerNode(" + node_a->to_string() + "**" + node_b->to_string() + ")";
}