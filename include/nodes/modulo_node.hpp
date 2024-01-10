#pragma once

#include "binary_operation.hpp"
using namespace std;

class ModuloNode: public BinaryOperationNode {
  public:
    ModuloNode(
      const CustomNode* a,
      const CustomNode* b
    );
    
    string to_string() const override;
};