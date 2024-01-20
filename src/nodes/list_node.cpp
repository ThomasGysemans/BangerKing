#include "../../include/nodes/list_node.hpp"

// Implement the constructor
ListNode::ListNode(
  unique_ptr<list<unique_ptr<CustomNode>>> nodes,
  const Position& start,
  const Position& end
): CustomNode(start, end), element_nodes(move(nodes)) { }

ListNode::ListNode(
  unique_ptr<list<unique_ptr<CustomNode>>> nodes
):
  CustomNode(
    nodes->front()->getStartingPosition(),
    nodes->back()->getEndingPosition()
  ), element_nodes(move(nodes)) {}

unique_ptr<list<unique_ptr<CustomNode>>> ListNode::get_element_nodes() {
  return move(element_nodes);
}

int ListNode::get_number_of_nodes() const {
  return element_nodes->size();
}

string ListNode::to_string() const {
  if (element_nodes->empty()) {
    return "[]";
  }
  list<unique_ptr<CustomNode>>::const_iterator iter = element_nodes->begin();
  string result = "[" + (*iter)->to_string(); // (or (**iter).to_string() it's the same thing
  ++iter;
  while (iter != element_nodes->end()) {
    result += ", " + (*iter)->to_string();
    ++iter;
  }
  return result + "]";
}