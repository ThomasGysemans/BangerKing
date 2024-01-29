#pragma once

#include "custom_node.hpp"

/// @brief Defines a mathematical operation involving two members (a and b).
/// It applies to additions, substractions, multiplications, etc.
class BinaryOperationNode: public CustomNode {
  protected:
    std::unique_ptr<CustomNode> node_a;
    std::unique_ptr<CustomNode> node_b;

    BinaryOperationNode(
      std::unique_ptr<CustomNode> a,
      std::unique_ptr<CustomNode> b
    );

  public:
    virtual ~BinaryOperationNode() override = default;

    std::unique_ptr<CustomNode> retrieve_a();
    std::unique_ptr<CustomNode> retrieve_b();
    virtual std::string to_string() const override = 0; // pure inherited virtual method
};