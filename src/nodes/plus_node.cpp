#include "../../include/nodes/plus_node.hpp"
using namespace std;

PlusNode::PlusNode(
  const CustomNode* n
):  CustomNode((*n).getStartingPosition(), (*n).getEndingPosition()), node(n) {}

const CustomNode* PlusNode::get_node() const { return node; }

string PlusNode::to_string() const {
  return "(+" + node->to_string() + ")";
}