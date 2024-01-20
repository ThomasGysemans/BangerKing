#include "../../include/nodes/multiply_node.hpp"

MultiplyNode::MultiplyNode(
  unique_ptr<CustomNode>& a,
  unique_ptr<CustomNode>& b
): BinaryOperationNode(move(a), move(b)) { }

string MultiplyNode::to_string() const {
  return "MultiplyNode(" + node_a->to_string() + "*" + node_b->to_string() + ")";
}