#pragma once

#include "binary_operation.hpp"
using namespace std;

class ModuloNode: public BinaryOperationNode {
  public:
    ModuloNode(
      unique_ptr<CustomNode>& a,
      unique_ptr<CustomNode>& b
    );
    
    string to_string() const override;
};