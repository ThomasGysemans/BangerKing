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
      std::unique_ptr<CustomNode> b,
      const NodeType::Type& type
    );

  public:
    // "virtual" here isn't necessary
    // it's redundant due to the "override" keyword.
    ~BinaryOperationNode() override = default;

    std::unique_ptr<CustomNode> retrieve_a();
    std::unique_ptr<CustomNode> retrieve_b();
    [[nodiscard]] std::string to_string() const override = 0; // pure inherited virtual method
};