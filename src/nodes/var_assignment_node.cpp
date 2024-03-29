#include "../../include/nodes/var_assignment_node.hpp"
#include "../../include/position.hpp"
using namespace std;

VarAssignmentNode::VarAssignmentNode(
  string var_name,
  unique_ptr<CustomNode> value,
  const Token& type_tok,
  const Position& pos_start,
  const Position& pos_end
)
: CustomNode(pos_start, pos_end, NodeType::VAR_ASSIGNMENT),
  var_name(move(var_name)),
  value_node(move(value)),
  type_name(type_tok.getStringValue()) {}

unique_ptr<CustomNode> VarAssignmentNode::retrieve_value_node() { return move(value_node); }
string VarAssignmentNode::get_var_name() const { return var_name; }
bool VarAssignmentNode::has_value() const { return value_node != nullptr; }
string VarAssignmentNode::get_type_name() const { return type_name; }
Type VarAssignmentNode::get_type() const { return get_type_from_name(type_name); }

string VarAssignmentNode::to_string() const {
  if (has_value()) {
    return "store " + var_name + " as " + type_name + " = " + value_node->to_string();
  } else {
    return "store " + var_name + " as " + type_name;
  }
}