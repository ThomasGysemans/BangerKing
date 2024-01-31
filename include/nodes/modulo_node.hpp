#pragma once

#include "binary_operation.hpp"

class ModuloNode final: public BinaryOperationNode {
  public:
    ModuloNode(
      std::unique_ptr<CustomNode>& a,
      std::unique_ptr<CustomNode>& b
    );
    
    [[nodiscard]] std::string to_string() const override;
};