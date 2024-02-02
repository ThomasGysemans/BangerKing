#include "../../include/nodes/multiply_node.hpp"
using namespace std;

MultiplyNode::MultiplyNode(
  unique_ptr<CustomNode>& a,
  unique_ptr<CustomNode>& b
): BinaryOperationNode(move(a), move(b), NodeType::MULTIPLY) { }

string MultiplyNode::to_string() const {
  return "MultiplyNode(" + node_a->to_string() + "*" + node_b->to_string() + ")";
}