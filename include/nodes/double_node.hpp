#pragma once

#include "custom_node.hpp"
#include "../token.hpp"

class DoubleNode: public CustomNode {
  const Token token;

  public:
    DoubleNode(const Token& token);

    const Token get_token() const;
    std::string to_string() const override;
};