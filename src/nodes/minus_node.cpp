#include "../../include/nodes/minus_node.hpp"
using namespace std;

MinusNode::MinusNode(
  const CustomNode* n
):  CustomNode(n->getStartingPosition(), n->getEndingPosition()), node(n) {}

MinusNode::~MinusNode() {
  delete node;
}

const CustomNode* MinusNode::get_node() const { return node; }

string MinusNode::to_string() const {
  return "(-" + node->to_string() + ")";
}