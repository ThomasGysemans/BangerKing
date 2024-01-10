#include "../../include/nodes/modulo_node.hpp"

ModuloNode::ModuloNode(
  const CustomNode* a,
  const CustomNode* b
): BinaryOperationNode(a, b) { }

string ModuloNode::to_string() const {
  return "ModuloNode(" + node_a->to_string() + "%" + node_b->to_string() + ")";
}