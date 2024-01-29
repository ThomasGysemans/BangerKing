#pragma once

#include "custom_node.hpp"
#include "../token.hpp"

class VarAccessNode: public CustomNode {
  const std::string var_name;

  public:
    VarAccessNode(
      const Token& var_tok
    );

    ~VarAccessNode() = default;

    std::string get_var_name() const;
    std::string to_string() const override;
};