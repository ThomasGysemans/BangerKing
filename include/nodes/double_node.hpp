#pragma once

#include "custom_node.hpp"
#include "../token.hpp"

class DoubleNode final: public CustomNode {
  const Token token;

  public:
    explicit DoubleNode(const Token& token);

    [[nodiscard]] Token get_token() const;
    [[nodiscard]] std::string to_string() const override;
};