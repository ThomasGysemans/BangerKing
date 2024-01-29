#pragma once

#include "binary_operation.hpp"

class DivideNode: public BinaryOperationNode {
  public:
    DivideNode(
      std::unique_ptr<CustomNode>& a,
      std::unique_ptr<CustomNode>& b
    );

    std::string to_string() const override;
};