#include "../../include/nodes/and_node.hpp"
using namespace std;

AndNode::AndNode(
  unique_ptr<CustomNode>& a,
  unique_ptr<CustomNode>& b
): BinaryOperationNode(move(a), move(b), NodeType::AND) { }

string AndNode::to_string() const {
  return "AndNode(" + node_a->to_string() + " and " + node_b->to_string() + ")";
}