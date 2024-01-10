#include "../../include/nodes/binary_operation.hpp"

BinaryOperationNode::BinaryOperationNode(
  const CustomNode* a,
  const CustomNode* b
): CustomNode(a->getStartingPosition(), b->getEndingPosition()), node_a(a), node_b(b) { }

BinaryOperationNode::~BinaryOperationNode() {
  delete node_a;
  delete node_b;
}

const CustomNode* BinaryOperationNode::get_a() const { return node_a; }
const CustomNode* BinaryOperationNode::get_b() const { return node_b; }
