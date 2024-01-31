#pragma once

#include "custom_node.hpp"
#include "../token.hpp"

class StringNode final: public CustomNode {
  const Token token;

  public:
    explicit StringNode(const Token&);

    /// @brief If the token is a string, can it include variables for concatenation?
    /// A string allowed for internal concatenation has double quotes.
    /// @return `true` if `allow_concatenation` of `token` is `true`, `false` otherwise.
    [[nodiscard]] bool canConcatenate() const;

    [[nodiscard]] const Token* getToken() const;
    [[nodiscard]] std::string getValue() const;
    [[nodiscard]] std::string to_string() const override;
};