#include "../../include/nodes/binary_operation.hpp"
using namespace std;

BinaryOperationNode::BinaryOperationNode(
  unique_ptr<CustomNode> a,
  unique_ptr<CustomNode> b
): CustomNode(a->getStartingPosition(), b->getEndingPosition()), node_a(move(a)), node_b(move(b)) { }

unique_ptr<CustomNode> BinaryOperationNode::retrieve_a() { return move(node_a); }
unique_ptr<CustomNode> BinaryOperationNode::retrieve_b() { return move(node_b); }
