#pragma once

#include "binary_operation.hpp"

class SubstractNode: public BinaryOperationNode {
  public:
    SubstractNode(
      std::unique_ptr<CustomNode>& a,
      std::unique_ptr<CustomNode>& b
    );

    std::string to_string() const override;
};