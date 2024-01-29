#include "../../include/nodes/divide_node.hpp"
using namespace std;

DivideNode::DivideNode(
  std::unique_ptr<CustomNode>& a,
  std::unique_ptr<CustomNode>& b
): BinaryOperationNode(move(a), move(b)) { }

string DivideNode::to_string() const {
  return "DivideNode(" + node_a->to_string() + "/" + node_b->to_string() + ")";
}