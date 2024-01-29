#include "../../include/nodes/plus_node.hpp"
using namespace std;

PlusNode::PlusNode(
  unique_ptr<CustomNode> n
): CustomNode(n->getStartingPosition(), n->getEndingPosition()), node(move(n)) {}

unique_ptr<CustomNode> PlusNode::retrieve_node() { return move(node); }

string PlusNode::to_string() const {
  return "(+" + node->to_string() + ")";
}