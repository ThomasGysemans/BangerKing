#include "../../include/nodes/add_node.hpp"

AddNode::AddNode(
  const CustomNode* a,
  const CustomNode* b
): CustomNode(a->getStartingPosition(), b->getEndingPosition()), node_a(a), node_b(b) {}

AddNode::~AddNode() {
  delete node_a;
  delete node_b;
}

const CustomNode* AddNode::get_a() const { return node_a; }
const CustomNode* AddNode::get_b() const { return node_b; }

string AddNode::to_string() const {
  return "AddNode(" + node_a->to_string() + "+" + node_b->to_string() + ")";
}