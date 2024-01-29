#pragma once

#include "custom_node.hpp"

// unary operation: +5

class PlusNode: public CustomNode {
  std::unique_ptr<CustomNode> node;

  public:
    PlusNode(std::unique_ptr<CustomNode> n);

    ~PlusNode() = default;

    /// @brief Gets the node it holds and transfers ownership.
    /// The node can be any type of node, a function, or a just a number.
    /// As a consequence, this method isn't constant since it modifies `node`.
    /// @return The ownership of the node it holds.
    std::unique_ptr<CustomNode> retrieve_node();
    std::string to_string() const override;
};