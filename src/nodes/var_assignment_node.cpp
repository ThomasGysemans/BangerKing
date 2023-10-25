#include "../../include/nodes/var_assignment_node.hpp"
#include "../../include/position.hpp"

VarAssignmentNode::VarAssignmentNode(
  const string& var_name,
  const CustomNode* value,
  const Token type_tok,
  const Position pos_start
)
: CustomNode(pos_start, value == nullptr ? pos_start : value->getEndingPosition()),
  var_name(var_name),
  value_node(value),
  type(get_type_from_name(type_tok.getStringValue())),
  type_name(type_tok.getStringValue()) {}

VarAssignmentNode::~VarAssignmentNode() {
  delete value_node;
}

const CustomNode* VarAssignmentNode::get_value_node() const { return value_node; }
string VarAssignmentNode::get_var_name() const { return var_name; }
bool VarAssignmentNode::has_value() const { return value_node != nullptr; }
Type VarAssignmentNode::get_type() const { return type; }
string VarAssignmentNode::get_type_name() const { return type_name; }

string VarAssignmentNode::to_string() const {
  return "store " + var_name + " as " + type_name + " = " + value_node->to_string();
}