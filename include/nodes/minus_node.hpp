#pragma once

#include "custom_node.hpp"
using namespace std;

// unary operation: -5

/// @brief Describes the negative unary operation.
/// Basically it will multiply a number, held in the `node` member, by -1.
class MinusNode: public CustomNode {
  unique_ptr<CustomNode> node;

  public:
    MinusNode(unique_ptr<CustomNode> n);

    ~MinusNode() = default;

    /// @brief Gets the node it holds and transfers ownership.
    /// The node can be any type of node, a function, or a just a number.
    /// As a consequence, this method isn't constant since it modifies `node`.
    /// @return The ownership of the node it holds.
    unique_ptr<CustomNode> get_node();

    string to_string() const override;
};