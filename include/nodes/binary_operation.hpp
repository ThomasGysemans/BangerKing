#pragma once

#include "custom_node.hpp"
using namespace std;

/// @brief Defines a mathematical operation involving two members (a and b).
/// It applies to additions, substractions, multiplications, etc.
class BinaryOperationNode: public CustomNode {
  protected:
    const CustomNode* node_a;
    const CustomNode* node_b;

    BinaryOperationNode(
      const CustomNode* a,
      const CustomNode* b
    );

  public:
    ~BinaryOperationNode();

    const CustomNode* get_a() const;
    const CustomNode* get_b() const;
    virtual string to_string() const override = 0; // pure inherited virtual method
};