#include "../../include/nodes/add_node.hpp"

AddNode::AddNode(
  unique_ptr<CustomNode>& a,
  unique_ptr<CustomNode>& b
): BinaryOperationNode(move(a), move(b)) { }

string AddNode::to_string() const {
  return "AddNode(" + node_a->to_string() + "+" + node_b->to_string() + ")";
}