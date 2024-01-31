#pragma once

#include "binary_operation.hpp"

class DivideNode final: public BinaryOperationNode {
  public:
    DivideNode(
      std::unique_ptr<CustomNode>& a,
      std::unique_ptr<CustomNode>& b
    );

    [[nodiscard]] std::string to_string() const override;
};