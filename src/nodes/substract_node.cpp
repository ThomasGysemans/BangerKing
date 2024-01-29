#include "../../include/nodes/substract_node.hpp"
using namespace std;

SubstractNode::SubstractNode(
  unique_ptr<CustomNode>& a,
  unique_ptr<CustomNode>& b
): BinaryOperationNode(move(a), move(b)) { }

string SubstractNode::to_string() const {
  return "SubstractNode(" + node_a->to_string() + "-" + node_b->to_string() + ")";
}