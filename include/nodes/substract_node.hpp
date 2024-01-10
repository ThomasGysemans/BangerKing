#pragma once

#include "binary_operation.hpp"
using namespace std;

class SubstractNode: public BinaryOperationNode {
  public:
    SubstractNode(
      const CustomNode* a,
      const CustomNode* b
    );

    string to_string() const override;
};