#include "../../include/nodes/power_node.hpp"

PowerNode::PowerNode(
  const CustomNode* a,
  const CustomNode* b
): CustomNode(a->getStartingPosition(), b->getEndingPosition()), node_a(a), node_b(b) {}

const CustomNode* PowerNode::get_a() const { return node_a; }
const CustomNode* PowerNode::get_b() const { return node_b; }

string PowerNode::to_string() const {
  return "PowerNode(" + node_a->to_string() + "**" + node_b->to_string() + ")";
}