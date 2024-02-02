#include "../../include/nodes/not_node.hpp"
using namespace std;

NotNode::NotNode(
  unique_ptr<CustomNode> n
): CustomNode(n->getStartingPosition(), n->getEndingPosition(), NodeType::NOT), node(move(n)) {}

unique_ptr<CustomNode> NotNode::retrieve_node() { return move(node); }

string NotNode::to_string() const {
  return "(!" + node->to_string() + ")";
}