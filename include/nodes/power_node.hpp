#pragma once

#include "binary_operation.hpp"

class PowerNode final: public BinaryOperationNode {
  public:
    PowerNode(
      std::unique_ptr<CustomNode>& a,
      std::unique_ptr<CustomNode>& b
    );

    [[nodiscard]] std::string to_string() const override;
};