#pragma once

#include "custom_node.hpp"
#include "../token.hpp"
using namespace std;

class DoubleNode: public CustomNode {
  // No need to bother with float values,
  // let's just use doubles for any decimal number.
  const double value;

  public:
    DoubleNode(const Token& token);

    double getValue() const;
    string to_string() const override;
};