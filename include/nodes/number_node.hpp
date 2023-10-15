#pragma once

#include "custom_node.hpp"
#include "../token.hpp"
using namespace std;

class NumberNode: public CustomNode {
  const int value;

  public:
    NumberNode(const Token& token);

    int getValue() const;
    string to_string() const override;
};