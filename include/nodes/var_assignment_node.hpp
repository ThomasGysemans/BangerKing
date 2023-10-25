#pragma once

#include "custom_node.hpp"
#include "../token.hpp"
#include "../types.hpp"
using namespace std;

class VarAssignmentNode: public CustomNode {
  const string var_name;
  const CustomNode* value_node; // can be "nullptr" if the variable doesn't have an initial value
  const Type type;
  const string type_name; // in case the type is the instance of a custom object

  public:
    VarAssignmentNode(
      const string& var_name,
      const CustomNode* value,
      const Token type_tok,
      const Position pos_start
    );

    ~VarAssignmentNode();

    const CustomNode* get_value_node() const;
    string get_var_name() const;
    bool has_value() const;
    Type get_type() const;
    string get_type_name() const;
    string to_string() const override;
};