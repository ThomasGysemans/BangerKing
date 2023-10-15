#include "../../include/nodes/divide_node.hpp"

DivideNode::DivideNode(
  const CustomNode* a,
  const CustomNode* b
): CustomNode(a->getStartingPosition(), b->getEndingPosition()), node_a(a), node_b(b) {}

const CustomNode* DivideNode::get_a() const { return node_a; }
const CustomNode* DivideNode::get_b() const { return node_b; }

string DivideNode::to_string() const {
  return "DivideNode(" + node_a->to_string() + "/" + node_b->to_string() + ")";
}