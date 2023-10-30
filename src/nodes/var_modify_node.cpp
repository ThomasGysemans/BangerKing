#include "../../include/nodes/var_modify_node.hpp"

VarModifyNode::VarModifyNode(
  const string& var_name,
  const CustomNode* value,
  const Position pos_start
): CustomNode(pos_start, value->getEndingPosition()), var_name(var_name), value_node(value) { }

VarModifyNode::~VarModifyNode() {
  delete value_node;
}

const CustomNode* VarModifyNode::get_value_node() const { return value_node; }
const string VarModifyNode::get_var_name() const { return var_name; }

string VarModifyNode::to_string() const {
  return var_name + " = " + value_node->to_string();
}