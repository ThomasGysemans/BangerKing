#include "../../include/nodes/custom_node.hpp"

CustomNode::CustomNode(
  const Position& start,
  const Position& end
): pos_start(start), pos_end(end) {}

const Position CustomNode::getStartingPosition() const { return pos_start; }
const Position CustomNode::getEndingPosition() const { return pos_end; }