#pragma once

#include "binary_operation.hpp"

class OrNode final: public BinaryOperationNode {
  public:
    OrNode(
      std::unique_ptr<CustomNode>& a,
      std::unique_ptr<CustomNode>& b
    );

    [[nodiscard]] std::string to_string() const override;
};