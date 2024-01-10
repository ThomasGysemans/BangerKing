#pragma once

#include "binary_operation.hpp"
using namespace std;

class PowerNode: public BinaryOperationNode {
  public:
    PowerNode(
      const CustomNode* a,
      const CustomNode* b
    );

    string to_string() const override;
};