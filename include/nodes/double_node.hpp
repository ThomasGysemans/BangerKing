#pragma once

#include "custom_node.hpp"
#include "../token.hpp"
using namespace std;

class DoubleNode: public CustomNode {
  const Token token;

  public:
    DoubleNode(const Token& token);

    const Token get_token() const;
    string to_string() const override;
};