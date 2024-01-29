#pragma once

#include "custom_node.hpp"
#include "../token.hpp"

class IntegerNode: public CustomNode {
  const Token token;

  public:
    IntegerNode(const Token& token);

    const Token get_token() const;
    std::string to_string() const override;
};