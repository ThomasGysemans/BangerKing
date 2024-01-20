#include "../../include/nodes/divide_node.hpp"

DivideNode::DivideNode(
  unique_ptr<CustomNode>& a,
  unique_ptr<CustomNode>& b
): BinaryOperationNode(move(a), move(b)) { }

string DivideNode::to_string() const {
  return "DivideNode(" + node_a->to_string() + "/" + node_b->to_string() + ")";
}