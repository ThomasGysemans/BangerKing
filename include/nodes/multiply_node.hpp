#pragma once

#include "binary_operation.hpp"
using namespace std;

class MultiplyNode: public BinaryOperationNode {
  public:
    MultiplyNode(
      const CustomNode* a,
      const CustomNode* b
    );

    string to_string() const override;
};