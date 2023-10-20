#pragma once

#include "custom_node.hpp"
using namespace std;

// unary operation: +5

class PlusNode: public CustomNode {
  const CustomNode* node;

  public:
    PlusNode(const CustomNode* n);

    ~PlusNode();

    const CustomNode* get_node() const;
    string to_string() const override;
};