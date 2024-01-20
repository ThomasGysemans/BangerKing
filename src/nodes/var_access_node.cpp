#include "../../include/nodes/var_access_node.hpp"

VarAccessNode::VarAccessNode(
  const Token& tok
): CustomNode(tok.getStartingPosition(), tok.getEndingPosition()), var_name(tok.getStringValue()) {}

string VarAccessNode::get_var_name() const { return var_name; }
string VarAccessNode::to_string() const { return "(" + var_name + ")"; }