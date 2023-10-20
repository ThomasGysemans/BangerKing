#include "../../include/nodes/modulo_node.hpp"

ModuloNode::ModuloNode(
  const CustomNode* a,
  const CustomNode* b
): CustomNode(a->getStartingPosition(), b->getEndingPosition()), node_a(a), node_b(b) {}

ModuloNode::~ModuloNode() {
  delete node_a;
  delete node_b;
}

const CustomNode* ModuloNode::get_a() const { return node_a; }
const CustomNode* ModuloNode::get_b() const { return node_b; }

string ModuloNode::to_string() const {
  return "ModuloNode(" + node_a->to_string() + "%" + node_b->to_string() + ")";
}