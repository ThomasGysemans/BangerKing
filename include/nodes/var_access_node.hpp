#pragma once

#include "custom_node.hpp"
#include "../token.hpp"

class VarAccessNode final: public CustomNode {
  const std::string var_name;

  public:
    explicit VarAccessNode(
      const Token& tok // the token leading to the variable name
    );

    ~VarAccessNode() override = default;

    [[nodiscard]] std::string get_var_name() const;
    [[nodiscard]] std::string to_string() const override;
};