#pragma once

#include "custom_node.hpp"
#include "../token.hpp"

class StringNode: public CustomNode {
  const Token token;

  public:
    StringNode(const Token n);

    /// @brief If the token is a string, can it include variables for concatenation?
    /// A string allowed for internal concatenation has double quotes.
    /// @return `true` if `allow_concatenation` of `token` is `true`, `false` otherwise.
    bool canConcatenate() const;

    const Token* getToken() const;
    const std::string getValue() const;
    std::string to_string() const override;
};