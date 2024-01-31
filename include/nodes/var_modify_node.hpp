#pragma once

#include "custom_node.hpp"
#include "../token.hpp"

class VarModifyNode final: public CustomNode {
  const std::string var_name;
  std::unique_ptr<CustomNode> value_node;

  public:
    VarModifyNode(
      std::string var_name,
      std::unique_ptr<CustomNode> value,
      const Position& pos_start
    );

    ~VarModifyNode() override = default;

    std::unique_ptr<CustomNode> retrieve_value_node();
    [[nodiscard]] std::string get_var_name() const;
    [[nodiscard]] std::string to_string() const override;
};