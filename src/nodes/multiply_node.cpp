#include "../../include/nodes/multiply_node.hpp"

MultiplyNode::MultiplyNode(
  const CustomNode* a,
  const CustomNode* b
): BinaryOperationNode(a, b) { }

string MultiplyNode::to_string() const {
  return "MultiplyNode(" + node_a->to_string() + "*" + node_b->to_string() + ")";
}