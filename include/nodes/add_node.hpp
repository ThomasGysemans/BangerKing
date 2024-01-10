#pragma once

#include "binary_operation.hpp"
using namespace std;

class AddNode: public BinaryOperationNode {
  public:
    AddNode(
      const CustomNode* a,
      const CustomNode* b
    );

    string to_string() const override;
};