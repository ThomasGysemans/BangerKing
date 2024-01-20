#pragma once

#include "custom_node.hpp"
#include "../token.hpp"
using namespace std;

class VarAccessNode: public CustomNode {
  const string var_name;

  public:
    VarAccessNode(
      const Token& var_tok
    );

    ~VarAccessNode() = default;

    string get_var_name() const;
    string to_string() const override;
};