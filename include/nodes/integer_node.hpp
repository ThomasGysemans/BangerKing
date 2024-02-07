#pragma once

#include "custom_node.hpp"
#include "../token.hpp"

class IntegerNode final: public CustomNode {
  const Token token;

  public:
    explicit IntegerNode(const Token& token);

    [[nodiscard]] Token get_token() const;
    [[nodiscard]] std::string to_string() const override;
    [[nodiscard]] std::string literal() const override;
};