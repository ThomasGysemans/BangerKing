#include "../../include/nodes/custom_node.hpp"
using namespace std;

CustomNode::CustomNode(
  Position start,
  Position end,
  const NodeType::Type& type
): pos_start(move(start)), pos_end(move(end)), type(type) {}

Position CustomNode::getStartingPosition() const { return pos_start; }
Position CustomNode::getEndingPosition() const { return pos_end; }
NodeType::Type CustomNode::getNodeType() const { return type; }
