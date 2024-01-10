#include "../../include/nodes/substract_node.hpp"

SubstractNode::SubstractNode(
  const CustomNode* a,
  const CustomNode* b
): BinaryOperationNode(a, b) { }

string SubstractNode::to_string() const {
  return "SubstractNode(" + node_a->to_string() + "-" + node_b->to_string() + ")";
}