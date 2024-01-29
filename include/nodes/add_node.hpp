#pragma once

#include "binary_operation.hpp"

class AddNode: public BinaryOperationNode {
  public:
    AddNode(
      std::unique_ptr<CustomNode>& a,
      std::unique_ptr<CustomNode>& b
    );

    std::string to_string() const override;
};