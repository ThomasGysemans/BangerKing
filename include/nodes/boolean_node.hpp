#pragma once

#include "custom_node.hpp"
#include "../token.hpp"

class BooleanNode final: public CustomNode {
  const Token token;

  public:
    explicit BooleanNode(const Token& token);

    ~BooleanNode() override = default;

    [[nodiscard]] bool is_true() const;
    [[nodiscard]] const Token* getToken() const;
    [[nodiscard]] std::string to_string() const override;
};