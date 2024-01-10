#pragma once

#include "binary_operation.hpp"
using namespace std;

class DivideNode: public BinaryOperationNode {
  public:
    DivideNode(
      const CustomNode* a,
      const CustomNode* b
    );

    string to_string() const override;
};