#include "../../include/nodes/list_node.hpp"

// ListNode::ListNode() {
//   list<const CustomNode*> nodes,
//   const Position& start,
//   const Position& end
// }: CustomNode(start, end), element_nodes(nodes) {}

// Implement the constructor
ListNode::ListNode(
  std::list<const CustomNode*> nodes,
  const Position& start,
  const Position& end
): CustomNode(start, end), element_nodes(nodes) { }

ListNode::~ListNode() {
  for (auto it = element_nodes.begin(); it != element_nodes.end(); ++it) {
    delete *it;
  }
}

list<const CustomNode*> ListNode::get_element_nodes() const { return element_nodes; }

string ListNode::to_string() const {
  list<const CustomNode*>::const_iterator iter = element_nodes.begin();
  string result = "[" + (*iter)->to_string(); // (or (**iter).to_string() it's the same thing
  ++iter;
  while (iter != element_nodes.end()) {
    result += ", " + (*iter)->to_string();
    ++iter;
  }
  return result + "]";
}