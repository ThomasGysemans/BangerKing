#pragma once

#include "custom_node.hpp"
using namespace std;

// unary operation: -5

class MinusNode: public CustomNode {
  const CustomNode* node;

  public:
    MinusNode(const CustomNode* n);

    ~MinusNode();

    const CustomNode* get_node() const;
    string to_string() const override;
};