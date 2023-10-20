#include "../../include/nodes/multiply_node.hpp"

MultiplyNode::MultiplyNode(
  const CustomNode* a,
  const CustomNode* b
): CustomNode(a->getStartingPosition(), b->getEndingPosition()), node_a(a), node_b(b) {}

MultiplyNode::~MultiplyNode() {
  delete node_a;
  delete node_b;
}

const CustomNode* MultiplyNode::get_a() const { return node_a; }
const CustomNode* MultiplyNode::get_b() const { return node_b; }

string MultiplyNode::to_string() const {
  return "MultiplyNode(" + node_a->to_string() + "*" + node_b->to_string() + ")";
}