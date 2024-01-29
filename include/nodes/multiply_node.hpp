#pragma once

#include "binary_operation.hpp"

class MultiplyNode: public BinaryOperationNode {
  public:
    MultiplyNode(
      std::unique_ptr<CustomNode>& a,
      std::unique_ptr<CustomNode>& b
    );

    std::string to_string() const override;
};