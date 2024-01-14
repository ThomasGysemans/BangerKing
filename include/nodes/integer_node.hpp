#pragma once

#include "custom_node.hpp"
#include "../token.hpp"
using namespace std;

class IntegerNode: public CustomNode {
  const int value;

  public:
    IntegerNode(const Token& token);

    int getValue() const;
    string to_string() const override;
};