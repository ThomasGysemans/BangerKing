#pragma once

#include "binary_operation.hpp"
using namespace std;

class AddNode: public BinaryOperationNode {
  public:
    AddNode(
      unique_ptr<CustomNode>& a,
      unique_ptr<CustomNode>& b
    );

    string to_string() const override;
};