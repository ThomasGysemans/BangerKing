#include "../../include/nodes/divide_node.hpp"

DivideNode::DivideNode(
  const CustomNode* a,
  const CustomNode* b
): BinaryOperationNode(a, b) { }

string DivideNode::to_string() const {
  return "DivideNode(" + node_a->to_string() + "/" + node_b->to_string() + ")";
}