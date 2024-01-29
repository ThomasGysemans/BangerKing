#pragma once

#include "binary_operation.hpp"

class OrNode: public BinaryOperationNode {
  public:
    OrNode(
      std::unique_ptr<CustomNode>& a,
      std::unique_ptr<CustomNode>& b
    );

    std::string to_string() const override;
};