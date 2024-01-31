#pragma once

#include <list>
#include "../position.hpp"
#include "custom_node.hpp"

using list_of_nodes_ptr = std::unique_ptr<std::list<std::unique_ptr<CustomNode>>>;

/// @brief A list of nodes. It can also contain the whole program, as it is just a list of nodes too.
class ListNode final: public CustomNode {
  // Note that the CustomNode instances inside of this list
  // cannot be constant, because during interpretation
  // the nodes get progressively deallocated hence
  // modifying some nodes that would contain smart pointers to others.
  list_of_nodes_ptr element_nodes;

  public:
    ListNode(
      list_of_nodes_ptr nodes,
      const Position& start,
      const Position& end
    );
    
    explicit ListNode(list_of_nodes_ptr nodes);

    ~ListNode() override = default;

    list_of_nodes_ptr get_element_nodes();
    [[nodiscard]] int get_number_of_nodes() const;
    [[nodiscard]] std::string to_string() const override;
};
