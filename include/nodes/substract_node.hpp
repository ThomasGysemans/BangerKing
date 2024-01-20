#pragma once

#include "binary_operation.hpp"
using namespace std;

class SubstractNode: public BinaryOperationNode {
  public:
    SubstractNode(
      unique_ptr<CustomNode>& a,
      unique_ptr<CustomNode>& b
    );

    string to_string() const override;
};