#include "../../include/nodes/var_modify_node.hpp"
using namespace std;

VarModifyNode::VarModifyNode(
  const string& var_name,
  unique_ptr<CustomNode> value,
  const Position pos_start
): CustomNode(pos_start, value->getEndingPosition()), var_name(var_name), value_node(move(value)) { }

unique_ptr<CustomNode> VarModifyNode::retrieve_value_node() { return move(value_node); }
const string VarModifyNode::get_var_name() const { return var_name; }

string VarModifyNode::to_string() const {
  return var_name + " = " + value_node->to_string();
}