#pragma once

#include "custom_node.hpp"
#include "../token.hpp"
#include "../types.hpp"

class VarModifyNode: public CustomNode {
  const std::string var_name;
  std::unique_ptr<CustomNode> value_node; // can be "nullptr" if the variable doesn't have an initial value

  public:
    VarModifyNode(
      const std::string& var_name,
      std::unique_ptr<CustomNode> value,
      const Position pos_start
    );

    ~VarModifyNode() = default;

    std::unique_ptr<CustomNode> retrieve_value_node();
    const std::string get_var_name() const;
    std::string to_string() const override;
};