#pragma once

#include "custom_node.hpp"
using namespace std;

/// @brief Defines a mathematical operation involving two members (a and b).
/// It applies to additions, substractions, multiplications, etc.
class BinaryOperationNode: public CustomNode {
  protected:
    unique_ptr<CustomNode> node_a;
    unique_ptr<CustomNode> node_b;

    BinaryOperationNode(
      unique_ptr<CustomNode> a,
      unique_ptr<CustomNode> b
    );

  public:
    virtual ~BinaryOperationNode() override = default;

    unique_ptr<CustomNode> retrieve_a();
    unique_ptr<CustomNode> retrieve_b();
    virtual string to_string() const override = 0; // pure inherited virtual method
};