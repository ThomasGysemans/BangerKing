#include "../../include/nodes/add_node.hpp"

AddNode::AddNode(
  const CustomNode* a,
  const CustomNode* b
): BinaryOperationNode(a, b) { }

string AddNode::to_string() const {
  return "AddNode(" + node_a->to_string() + "+" + node_b->to_string() + ")";
}