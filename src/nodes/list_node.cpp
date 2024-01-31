#include "../../include/nodes/list_node.hpp"
using namespace std;

// Implement the constructor
ListNode::ListNode(
  list_of_nodes_ptr nodes,
  const Position& start,
  const Position& end
): CustomNode(start, end), element_nodes(move(nodes)) { }

ListNode::ListNode(
  list_of_nodes_ptr nodes
):
  CustomNode(
    nodes->front()->getStartingPosition(),
    nodes->back()->getEndingPosition()
  ), element_nodes(move(nodes)) {}

list_of_nodes_ptr ListNode::get_element_nodes() {
  return move(element_nodes);
}

int ListNode::get_number_of_nodes() const {
  return static_cast<int>(element_nodes->size());
}

string ListNode::to_string() const {
  if (element_nodes->empty()) {
    return "[]";
  }
  auto iter = element_nodes->begin();
  string result = "[" + (*iter)->to_string(); // (or (**iter).to_string() it's the same thing
  ++iter;
  while (iter != element_nodes->end()) {
    result += ", " + (*iter)->to_string();
    ++iter;
  }
  return result + "]";
}