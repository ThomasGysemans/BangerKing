#pragma once

#include "custom_node.hpp"
#include "../token.hpp"
#include "../types.hpp"
using namespace std;

class VarModifyNode: public CustomNode {
  const string var_name;
  unique_ptr<CustomNode> value_node; // can be "nullptr" if the variable doesn't have an initial value

  public:
    VarModifyNode(
      const string& var_name,
      unique_ptr<CustomNode> value,
      const Position pos_start
    );

    ~VarModifyNode() = default;

    unique_ptr<CustomNode> get_value_node();
    const string get_var_name() const;
    string to_string() const override;
};