#include "../../include/nodes/substract_node.hpp"

SubstractNode::SubstractNode(
  const CustomNode* a,
  const CustomNode* b
): CustomNode(a->getStartingPosition(), b->getEndingPosition()), node_a(a), node_b(b) {}

const CustomNode* SubstractNode::get_a() const { return node_a; }
const CustomNode* SubstractNode::get_b() const { return node_b; }

string SubstractNode::to_string() const {
  return "SubstractNode(" + node_a->to_string() + "-" + node_b->to_string() + ")";
}