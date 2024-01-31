#include "../../include/nodes/define_constant_node.hpp"
#include "../../include/position.hpp"
using namespace std;

DefineConstantNode::DefineConstantNode(
  string var_name,
  unique_ptr<CustomNode> value,
  const Type& type,
  const Position& pos_start,
  const Position& pos_end
)
: CustomNode(pos_start, pos_end),
  var_name(move(var_name)),
  value_node(move(value)),
  type(type) {}

unique_ptr<CustomNode> DefineConstantNode::retrieve_value_node() { return move(value_node); }
string DefineConstantNode::get_var_name() const { return var_name; }
Type DefineConstantNode::get_type() const { return type; }

string DefineConstantNode::to_string() const {
  return "define " + var_name + " as " + get_type_name(type) + " = " + value_node->to_string();
}