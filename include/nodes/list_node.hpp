#pragma once

#include <list>
#include "../position.hpp"
#include "custom_node.hpp"
using namespace std;

/// @brief A list of nodes. It can also contain the whole program, as it is just a list of nodes too.
class ListNode: public CustomNode {
  // A copy of the statements produced by the Parser
  // is necessary as the reference to the list created inside the parser
  // is lost when reaching the end of the process. 
  list<const CustomNode*> element_nodes;

  public:
    ListNode(
      list<const CustomNode*> nodes,
      const Position& start,
      const Position& end
    );
    
    ListNode(list<const CustomNode*> nodes);

    ~ListNode();

    list<const CustomNode*> get_element_nodes() const;
    int get_number_of_nodes() const;
    string to_string() const override;
};
