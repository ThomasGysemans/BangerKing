#pragma once

#include "binary_operation.hpp"

class SubstractNode final: public BinaryOperationNode {
  public:
    SubstractNode(
      std::unique_ptr<CustomNode>& a,
      std::unique_ptr<CustomNode>& b
    );

    [[nodiscard]] std::string to_string() const override;
};