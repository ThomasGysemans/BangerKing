#pragma once

#include "binary_operation.hpp"

class AndNode final: public BinaryOperationNode {
  public:
    AndNode(
      std::unique_ptr<CustomNode>& a,
      std::unique_ptr<CustomNode>& b
    );

    [[nodiscard]] std::string to_string() const override;
};