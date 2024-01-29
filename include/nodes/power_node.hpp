#pragma once

#include "binary_operation.hpp"

class PowerNode: public BinaryOperationNode {
  public:
    PowerNode(
      std::unique_ptr<CustomNode>& a,
      std::unique_ptr<CustomNode>& b
    );

    std::string to_string() const override;
};