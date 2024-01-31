#pragma once

#include "binary_operation.hpp"

// "final" here means
// that no other class
// can derive from "AddNode".
class AddNode final: public BinaryOperationNode {
  public:
    AddNode(
      std::unique_ptr<CustomNode>& a,
      std::unique_ptr<CustomNode>& b
    );

    [[nodiscard]] std::string to_string() const override;
};