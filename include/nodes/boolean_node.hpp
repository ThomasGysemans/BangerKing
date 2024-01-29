#pragma once

#include "custom_node.hpp"
#include "../token.hpp"

class BooleanNode: public CustomNode {
  const Token token;

  public:
    BooleanNode(const Token token);

    ~BooleanNode() = default;

    bool is_true() const;
    const Token* getToken() const;
    std::string to_string() const override;
};