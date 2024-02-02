#include "../../include/nodes/modulo_node.hpp"
using namespace std;

ModuloNode::ModuloNode(
  unique_ptr<CustomNode>& a,
  unique_ptr<CustomNode>& b
): BinaryOperationNode(move(a), move(b), NodeType::MODULO) { }

string ModuloNode::to_string() const {
  return "ModuloNode(" + node_a->to_string() + "%" + node_b->to_string() + ")";
}