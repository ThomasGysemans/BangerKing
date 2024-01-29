#pragma once

#include "binary_operation.hpp"

class AndNode: public BinaryOperationNode {
  public:
    AndNode(
      std::unique_ptr<CustomNode>& a,
      std::unique_ptr<CustomNode>& b
    );

    std::string to_string() const override;
};