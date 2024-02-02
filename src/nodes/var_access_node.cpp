#include "../../include/nodes/var_access_node.hpp"
using namespace std;

VarAccessNode::VarAccessNode(
  const Token& tok
): CustomNode(tok.getStartingPosition(), tok.getEndingPosition(), NodeType::VAR_ACCESS), var_name(tok.getStringValue()) {}

string VarAccessNode::get_var_name() const { return var_name; }
string VarAccessNode::to_string() const { return "(" + var_name + ")"; }