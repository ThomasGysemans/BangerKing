#pragma once

#include <list>
#include "../position.hpp"
#include "custom_node.hpp"
using namespace std;

/// @brief A list of nodes. It can also contain the whole program, as it is just a list of nodes too.
class ListNode: public CustomNode {
  // Note that the CustomNode instances inside of this list
  // cannot be constant, because during interpretation
  // the nodes get progressively deallocated hence
  // modifying some nodes that would contain smart pointers to others.
  unique_ptr<list<unique_ptr<CustomNode>>> element_nodes;

  public:
    ListNode(
      unique_ptr<list<unique_ptr<CustomNode>>> nodes,
      const Position& start,
      const Position& end
    );
    
    ListNode(unique_ptr<list<unique_ptr<CustomNode>>> nodes);

    ~ListNode() = default;

    unique_ptr<list<unique_ptr<CustomNode>>> get_element_nodes();
    int get_number_of_nodes() const;
    string to_string() const override;
};
