#pragma once

#include "custom_node.hpp"
#include "../token.hpp"
using namespace std;

class IntegerNode: public CustomNode {
  const Token token;

  public:
    IntegerNode(const Token& token);

    const Token get_token() const;
    string to_string() const override;
};