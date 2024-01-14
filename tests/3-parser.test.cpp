#include <iostream>
#include <list>
#include "helper.hpp"
#include "../include/token.hpp"
#include "../include/lexer.hpp"
#include "../include/parser.hpp"
#include "../include/nodes/compositer.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/debug/print_tokens.hpp"
using namespace std;

list<const CustomNode*> get_element_nodes_from(const string& code, bool debug_print = false) {
  Lexer lexer(&code);
  const list<Token*> tokens = lexer.generate_tokens();
  if (debug_print) {
    cout << "Resulft of lexer :" << endl;
    cout << display_tokens_list(tokens) << endl;
  }
  Parser parser(tokens);
  const ListNode* parsing_result = parser.parse();
  if (debug_print) {
    cout << "Result of parsing :" << endl;
    cout << parsing_result->to_string() << endl;
  }
  return parsing_result->get_element_nodes();
}

void test_simple_number() {
  const auto element_nodes = get_element_nodes_from("5");
  const auto first_element = element_nodes.front();
  const auto number_node = cast_node<IntegerNode>(first_element);

  assert(element_nodes.size() == 1);
  assert(number_node->getValue() == 5);

  delete number_node;

  print_success_msg("parser with simple number", 1);
}

void test_simple_decimal_number() {
  const auto element_nodes = get_element_nodes_from("3.14");
  const auto first_element = element_nodes.front();
  const auto number_node = cast_node<DoubleNode>(first_element);

  assert(element_nodes.size() == 1);
  assert(number_node->getValue() == 3.14);
  assert(number_node->to_string() == "DoubleNode(3.14)"); // to make sure the double is printed out correctly

  delete number_node;

  print_success_msg("parser with simple decimal number", 1);
}

void test_simple_negative_number() {
  const auto nodes = get_element_nodes_from("-5");
  const auto first_element = nodes.front();
  const auto minus_node = cast_node<MinusNode>(first_element);
  const auto number_node = cast_node<IntegerNode>(minus_node->get_node());
  assert(nodes.size() == 1);
  assert(number_node->getValue() == 5);
  
  delete minus_node;

  print_success_msg("parser with simple negative number", 1);
}

void test_simple_positive_number() {
  const auto nodes = get_element_nodes_from("+670");
  const auto first_element = nodes.front();
  const auto plus_node = cast_node<PlusNode>(first_element);
  const auto number_node = cast_node<IntegerNode>(plus_node->get_node());
  assert(nodes.size() == 1);
  assert(number_node->getValue() == 670);

  delete plus_node;

  print_success_msg("parser with simple positive number", 1);
}

void test_simple_addition() {
  const auto nodes = get_element_nodes_from("5+6");
  const auto first_element = nodes.front();
  const auto add_node = cast_node<AddNode>(first_element);
  const auto a = cast_node<IntegerNode>(add_node->get_a());
  const auto b = cast_node<IntegerNode>(add_node->get_b());
  assert(nodes.size() == 1);
  assert(a->getValue() == 5);
  assert(b->getValue() == 6);

  delete add_node;

  print_success_msg("parser with simple addition", 1);
}

void test_simple_addition_with_whitespace() {
  const auto nodes = get_element_nodes_from("5 + 6");
  const auto first_element = nodes.front();
  const auto add_node = cast_node<AddNode>(first_element);
  const auto a = cast_node<IntegerNode>(add_node->get_a());
  const auto b = cast_node<IntegerNode>(add_node->get_b());
  assert(nodes.size() == 1);
  assert(a->getValue() == 5);
  assert(b->getValue() == 6);

  delete add_node;

  print_success_msg("parser with simple addition and whitespace", 1);
}

void test_simple_substraction() {
  const auto nodes = get_element_nodes_from("0-6");
  const auto first_element = nodes.front();
  const auto sub_node = cast_node<SubstractNode>(first_element);
  const auto a = cast_node<IntegerNode>(sub_node->get_a());
  const auto b = cast_node<IntegerNode>(sub_node->get_b());
  assert(nodes.size() == 1);
  assert(a->getValue() == 0);
  assert(b->getValue() == 6);

  delete sub_node;

  print_success_msg("parser with simple substraction", 1);
}

void test_simple_substraction_with_whitespace() {
  const auto nodes = get_element_nodes_from("0 - 6");
  const auto first_element = nodes.front();
  const auto sub_node = cast_node<SubstractNode>(first_element);
  const auto a = cast_node<IntegerNode>(sub_node->get_a());
  const auto b = cast_node<IntegerNode>(sub_node->get_b());
  assert(nodes.size() == 1);
  assert(a->getValue() == 0);
  assert(b->getValue() == 6);

  delete sub_node;

  print_success_msg("parser with simple substraction and whitespace", 1);
}

void test_simple_multiplication() {
  const auto nodes = get_element_nodes_from("0*6");
  const auto first_element = nodes.front();
  const auto mul_node = cast_node<MultiplyNode>(first_element);
  const auto a = cast_node<IntegerNode>(mul_node->get_a());
  const auto b = cast_node<IntegerNode>(mul_node->get_b());
  assert(nodes.size() == 1);
  assert(a->getValue() == 0);
  assert(b->getValue() == 6);

  delete mul_node;

  print_success_msg("parser with simple multiplication", 1);
}

void test_simple_divison() {
  const auto nodes = get_element_nodes_from("10/2");
  const auto first_element = nodes.front();
  const auto div_node = cast_node<DivideNode>(first_element);
  const auto a = cast_node<IntegerNode>(div_node->get_a());
  const auto b = cast_node<IntegerNode>(div_node->get_b());
  assert(nodes.size() == 1);
  assert(a->getValue() == 10);
  assert(b->getValue() == 2);

  delete div_node;

  print_success_msg("parser with simple divison", 1);
}

void test_simple_modulo() {
  const auto nodes = get_element_nodes_from("13%12");
  const auto first_element = nodes.front();
  const auto modulo_node = cast_node<ModuloNode>(first_element);
  const auto a = cast_node<IntegerNode>(modulo_node->get_a());
  const auto b = cast_node<IntegerNode>(modulo_node->get_b());
  assert(nodes.size() == 1);
  assert(a->getValue() == 13);
  assert(b->getValue() == 12);

  delete modulo_node;

  print_success_msg("parser with simple modulo", 1);
}

void test_simple_power() {
  const auto nodes = get_element_nodes_from("10**2");
  const auto first_element = nodes.front();
  const auto power_node = cast_node<PowerNode>(first_element);
  const auto a = cast_node<IntegerNode>(power_node->get_a());
  const auto b = cast_node<IntegerNode>(power_node->get_b());
  assert(nodes.size() == 1);
  assert(a->getValue() == 10);
  assert(b->getValue() == 2);

  delete power_node;

  print_success_msg("parser with simple power", 1);
}

void test_complex_maths_expression_with_parenthesis() {
  const auto nodes = get_element_nodes_from("10+(5-2)/6"); // AddNode ( IntegerNode(10), DivideNode ( SubstractNode, IntegerNode(6) ) )
  assert(nodes.size() == 1);

  const auto first_element = nodes.front();
  const auto add_node = cast_node<AddNode>(first_element);
  const auto add_a = cast_node<IntegerNode>(add_node->get_a()); // 10
  const auto add_b = cast_node<DivideNode>(add_node->get_b()); // DivideNode( SubstractNode(), IntegerNode(10) )
  assert(add_a->getValue() == 10);

  const auto sub_node = cast_node<SubstractNode>(add_b->get_a());
  const auto sub_a = cast_node<IntegerNode>(sub_node->get_a());
  const auto sub_b = cast_node<IntegerNode>(sub_node->get_b());
  assert(sub_a->getValue() == 5);
  assert(sub_b->getValue() == 2);

  const auto den = cast_node<IntegerNode>(add_b->get_b());
  assert(den->getValue() == 6);

  delete add_node; // will delete everything it contains

  print_success_msg("parser with complex maths expression with parenthesis", 1);
}

void test_complex_maths_expression_without_parenthesis() {
  const auto nodes = get_element_nodes_from("10+5-2/6"); // SubstractNode( AddNode(10, 5), DivideNode(2, 6) )
  assert(nodes.size() == 1);

  const auto first_element = nodes.front();
  const auto sub_node = cast_node<SubstractNode>(first_element);
  const auto add_node = cast_node<AddNode>(sub_node->get_a());
  const auto div_node = cast_node<DivideNode>(sub_node->get_b());

  const auto add_a = cast_node<IntegerNode>(add_node->get_a());
  const auto add_b = cast_node<IntegerNode>(add_node->get_b());
  assert(add_a->getValue() == 10);
  assert(add_b->getValue() == 5);

  const auto div_a = cast_node<IntegerNode>(div_node->get_a());
  const auto div_b = cast_node<IntegerNode>(div_node->get_b());
  assert(div_a->getValue() == 2);
  assert(div_b->getValue() == 6);

  delete sub_node;

  print_success_msg("parser with complex maths expression without parenthesis", 1);
}

void test_mutiple_lines() {
  const auto nodes = get_element_nodes_from(
    "1+2\n"
    "3-4"
  );
  assert(nodes.size() == 2);

  const auto first_node = cast_node<AddNode>(nodes.front());
  const auto second_node = cast_node<SubstractNode>(nodes.back());

  assert(cast_node<IntegerNode>(first_node->get_a())->getValue() == 1);
  assert(cast_node<IntegerNode>(first_node->get_b())->getValue() == 2);
  assert(cast_node<IntegerNode>(second_node->get_a())->getValue() == 3);
  assert(cast_node<IntegerNode>(second_node->get_b())->getValue() == 4);

  delete first_node;
  delete second_node;

  print_success_msg("parser with multiple lines", 1);
}

void test_positions_on_single_digit() {
  const auto nodes = get_element_nodes_from("5");
  assert(nodes.size() == 1);

  const auto node = cast_node<IntegerNode>(nodes.front());
  const auto pos_start = node->getStartingPosition();
  const auto pos_end = node->getEndingPosition();

  assert(pos_start.get_idx() == 0);
  assert(pos_start.get_col() == 0);
  assert(pos_start.get_ln() == 0);

  assert(pos_end.get_idx() == 1);
  assert(pos_end.get_col() == 1);
  assert(pos_end.get_ln() == 0);

  print_success_msg("node's positions are valid on a single digit", 1);
}

void test_positions_on_math_expression() {
  const auto nodes = get_element_nodes_from("5+6-2");
  assert(nodes.size() == 1);

  const auto sub_node = cast_node<SubstractNode>(nodes.front());
  const auto add_node = cast_node<AddNode>(sub_node->get_a());
  const auto two = cast_node<IntegerNode>(sub_node->get_b());

  const auto five = cast_node<IntegerNode>(add_node->get_a());
  const auto six = cast_node<IntegerNode>(add_node->get_b());

  assert(five->getValue() == 5);
  assert(six->getValue() == 6);
  assert(two->getValue() == 2);

  assert(five->getStartingPosition().get_idx() == 0);
  assert(five->getEndingPosition().get_idx() == 1);
  assert(six->getStartingPosition().get_idx() == 2);
  assert(six->getEndingPosition().get_idx() == 3);
  assert(two->getStartingPosition().get_idx() == 4);
  assert(two->getEndingPosition().get_idx() == 5);

  assert(sub_node->getStartingPosition().get_idx() == five->getStartingPosition().get_idx());
  assert(sub_node->getEndingPosition().get_idx() == two->getEndingPosition().get_idx());
  assert(add_node->getStartingPosition().get_idx() == five->getStartingPosition().get_idx());
  assert(add_node->getEndingPosition().get_idx() == six->getEndingPosition().get_idx());

  delete sub_node;

  print_success_msg("node's positions are valid on a math expression", 1);
}

void test_variable_assignment_with_initial_value() {
  const string code = "store a as int = 5";
  const auto nodes = get_element_nodes_from(code);
  assert(nodes.size() == 1);

  const auto node = cast_node<VarAssignmentNode>(nodes.front());
  assert(node->get_type() == Type::INT);
  assert(node->get_type_name() == "int");
  assert(node->get_var_name() == "a");
  assert(node->has_value());

  assert(node->getStartingPosition().get_idx() == 0);
  assert(node->getEndingPosition().get_idx() == code.size());

  assert(node->get_value_node()->getStartingPosition().get_idx() == code.size() - 1);
  assert(node->get_value_node()->getEndingPosition().get_idx() == code.size());

  delete node;

  print_success_msg("variable assignment of an integer with initial value", 1);
}

void test_variable_assignment_without_initial_value() {
  const string code = "store a as int";
  const auto nodes = get_element_nodes_from(code);
  assert(nodes.size() == 1);

  const auto node = cast_node<VarAssignmentNode>(nodes.front());
  assert(node->get_type() == Type::INT);
  assert(node->get_type_name() == "int");
  assert(node->get_var_name() == "a");
  assert(!node->has_value());

  assert(node->getStartingPosition().get_idx() == 0);
  assert(node->getEndingPosition().get_idx() == code.size());

  delete node;

  print_success_msg("variable assignment of an integer without initial value", 1);
}

void test_variable_modification() {
  const string code = "a = 5";
  const auto nodes = get_element_nodes_from(code);
  assert(nodes.size() == 1);

  const auto var_modify_node = cast_node<VarModifyNode>(nodes.front());
  assert(var_modify_node->get_var_name() == "a");
  assert(instanceof<IntegerNode>(var_modify_node->get_value_node()));

  const auto integer = cast_node<IntegerNode>(var_modify_node->get_value_node());
  assert(integer->getValue() == 5);

  assert(var_modify_node->getStartingPosition().get_idx() == 0);
  assert(var_modify_node->getEndingPosition().get_idx() == code.size());
  assert(integer->getStartingPosition().get_idx() == 4);
  assert(integer->getEndingPosition().get_idx() == 5);

  delete var_modify_node; // will delete "integer" too.

  print_success_msg("variable modification", 1);
}

void test_access_node() {
  const string code = "variable";
  const auto nodes = get_element_nodes_from(code);
  assert(nodes.size() == 1);

  const auto node = cast_node<VarAccessNode>(nodes.front());
  assert(node->get_var_name() == "variable");
  assert(node->to_string() == "(variable)");
  assert(node->getStartingPosition().get_idx() == 0);
  assert(node->getEndingPosition().get_idx() == code.size());

  delete node;

  print_success_msg("variable access", 1);
}

void test_access_node_in_expression() {
  const string code = "a+5";
  const auto nodes = get_element_nodes_from(code);
  assert(nodes.size() == 1);
  assert(instanceof<AddNode>(nodes.front()));

  const auto node = cast_node<AddNode>(nodes.front());
  assert(instanceof<VarAccessNode>(node->get_a()));
  assert(instanceof<IntegerNode>(node->get_b()));
  assert(node->getStartingPosition().get_idx() == 0);
  assert(node->getEndingPosition().get_idx() == code.size());

  delete node;

  print_success_msg("variable access in maths expression", 1);
}

int main() {
  print_title("Parser tests...");

  try {
    test_simple_number();
    test_simple_decimal_number();
    test_simple_negative_number();
    test_simple_positive_number();
    test_simple_addition();
    test_simple_addition_with_whitespace();
    test_simple_substraction();
    test_simple_substraction_with_whitespace();
    test_simple_multiplication();
    test_simple_divison();
    test_simple_modulo();
    test_simple_power();
    test_complex_maths_expression_with_parenthesis();
    test_complex_maths_expression_without_parenthesis();
    test_mutiple_lines();
    test_positions_on_single_digit();
    test_positions_on_math_expression();
    test_variable_assignment_with_initial_value();
    test_variable_assignment_without_initial_value();
    test_variable_modification();
    test_access_node();
    test_access_node_in_expression();
  } catch (CustomError custom_error) {
    cerr << "Oops, the program unexpectedly thrown an error :" << endl;
    cerr << custom_error.get_details() << endl;
    return 1;
  } catch (string cast_exception) {
    cerr << "ABORT. Terminating tests due to this error:" << endl;
    cerr << cast_exception << endl;
    return 1;
  }

  print_success_msg("All \"Parser\" tests successfully passed");
  return 0;
}
