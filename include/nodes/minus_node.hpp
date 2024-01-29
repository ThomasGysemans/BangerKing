#pragma once

#include "custom_node.hpp"

// unary operation: -5

/// @brief Describes the negative unary operation.
/// Basically it will multiply a number, held in the `node` member, by -1.
class MinusNode: public CustomNode {
  std::unique_ptr<CustomNode> node;

  public:
    MinusNode(std::unique_ptr<CustomNode> n);

    ~MinusNode() = default;

    /// @brief Gets the node it holds and transfers ownership.
    /// The node can be any type of node, a function, or a just a number.
    /// As a consequence, this method isn't constant since it modifies `node`.
    /// @return The ownership of the node it holds.
    std::unique_ptr<CustomNode> retrieve_node();

    std::string to_string() const override;
};