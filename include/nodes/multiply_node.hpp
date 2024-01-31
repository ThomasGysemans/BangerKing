#pragma once

#include "binary_operation.hpp"

class MultiplyNode final: public BinaryOperationNode {
  public:
    MultiplyNode(
      std::unique_ptr<CustomNode>& a,
      std::unique_ptr<CustomNode>& b
    );

    [[nodiscard]] std::string to_string() const override;
};