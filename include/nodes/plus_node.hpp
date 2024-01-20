#pragma once

#include "custom_node.hpp"
using namespace std;

// unary operation: +5

class PlusNode: public CustomNode {
  unique_ptr<CustomNode> node;

  public:
    PlusNode(unique_ptr<CustomNode> n);

    ~PlusNode() = default;

    /// @brief Gets the node it holds and transfers ownership.
    /// The node can be any type of node, a function, or a just a number.
    /// As a consequence, this method isn't constant since it modifies `node`.
    /// @return The ownership of the node it holds.
    unique_ptr<CustomNode> get_node();
    string to_string() const override;
};