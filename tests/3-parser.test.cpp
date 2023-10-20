#include <iostream>
#include <list>
#include "helper.hpp"
#include "../include/token.hpp"
#include "../include/lexer.hpp"
#include "../include/parser.hpp"
#include "../include/nodes/compositer.hpp"
#include "../include/miscellaneous.hpp"
using namespace std;

list<const CustomNode*> get_element_nodes_from(const string& code, bool debug_print = false) {
  Lexer lexer(code);
  list<Token*> tokens = lexer.generate_tokens();
  Parser parser(tokens);
  const ListNode* parsing_result = parser.parse();
  if (debug_print) {
    cout << "Result of parsing :" << endl;
    cout << parsing_result->to_string() << endl;
  }
  return parsing_result->get_element_nodes();
}

void test_simple_number() {
  auto element_nodes = get_element_nodes_from("5");
  auto first_element = element_nodes.front();
  auto number_node = cast_node<NumberNode>(first_element);

  assert(element_nodes.size() == 1);
  assert(number_node->getValue() == 5);

  print_success_msg("parser with simple number", 1);
}

void test_simple_negative_number() {
  auto nodes = get_element_nodes_from("-5");
  auto first_element = nodes.front();
  auto minus_node = cast_node<MinusNode>(first_element);
  auto number_node = cast_node<NumberNode>(minus_node->get_node());
  assert(nodes.size() == 1);
  assert(number_node->getValue() == 5);

  print_success_msg("parser with simple negative number", 1);
}

void test_simple_positive_number() {
  auto nodes = get_element_nodes_from("+670");
  auto first_element = nodes.front();
  auto plus_node = cast_node<PlusNode>(first_element);
  auto number_node = cast_node<NumberNode>(plus_node->get_node());
  assert(nodes.size() == 1);
  assert(number_node->getValue() == 670);

  print_success_msg("parser with simple positive number", 1);
}

void test_simple_addition() {
  auto nodes = get_element_nodes_from("5+6");
  auto first_element = nodes.front();
  auto add_node = cast_node<AddNode>(first_element);
  auto a = cast_node<NumberNode>(add_node->get_a());
  auto b = cast_node<NumberNode>(add_node->get_b());
  assert(nodes.size() == 1);
  assert(a->getValue() == 5);
  assert(b->getValue() == 6);

  print_success_msg("parser with simple addition", 1);
}

void test_simple_substraction() {
  auto nodes = get_element_nodes_from("0-6");
  auto first_element = nodes.front();
  auto sub_node = cast_node<SubstractNode>(first_element);
  auto a = cast_node<NumberNode>(sub_node->get_a());
  auto b = cast_node<NumberNode>(sub_node->get_b());
  assert(nodes.size() == 1);
  assert(a->getValue() == 0);
  assert(b->getValue() == 6);

  print_success_msg("parser with simple substraction", 1);
}

void test_simple_multiplication() {
  auto nodes = get_element_nodes_from("0*6");
  auto first_element = nodes.front();
  auto mul_node = cast_node<MultiplyNode>(first_element);
  auto a = cast_node<NumberNode>(mul_node->get_a());
  auto b = cast_node<NumberNode>(mul_node->get_b());
  assert(nodes.size() == 1);
  assert(a->getValue() == 0);
  assert(b->getValue() == 6);

  print_success_msg("parser with simple multiplication", 1);
}

void test_simple_divison() {
  auto nodes = get_element_nodes_from("10/2");
  auto first_element = nodes.front();
  auto div_node = cast_node<DivideNode>(first_element);
  auto a = cast_node<NumberNode>(div_node->get_a());
  auto b = cast_node<NumberNode>(div_node->get_b());
  assert(nodes.size() == 1);
  assert(a->getValue() == 10);
  assert(b->getValue() == 2);

  print_success_msg("parser with simple divison", 1);
}

void test_simple_modulo() {
  auto nodes = get_element_nodes_from("13%12");
  auto first_element = nodes.front();
  auto modulo_node = cast_node<ModuloNode>(first_element);
  auto a = cast_node<NumberNode>(modulo_node->get_a());
  auto b = cast_node<NumberNode>(modulo_node->get_b());
  assert(nodes.size() == 1);
  assert(a->getValue() == 13);
  assert(b->getValue() == 12);

  print_success_msg("parser with simple modulo", 1);
}

void test_simple_power() {
  auto nodes = get_element_nodes_from("10**2");
  auto first_element = nodes.front();
  auto power_node = cast_node<PowerNode>(first_element);
  auto a = cast_node<NumberNode>(power_node->get_a());
  auto b = cast_node<NumberNode>(power_node->get_b());
  assert(nodes.size() == 1);
  assert(a->getValue() == 10);
  assert(b->getValue() == 2);

  print_success_msg("parser with simple power", 1);
}

void test_complex_maths_expression_with_parenthesis() {
  auto nodes = get_element_nodes_from("10+(5-2)/6"); // AddNode ( NumberNode(10), DivideNode ( SubstractNode, NumberNode(6) ) )
  assert(nodes.size() == 1);

  auto first_element = nodes.front();
  auto add_node = cast_node<AddNode>(first_element);
  auto add_a = cast_node<NumberNode>(add_node->get_a()); // 10
  auto add_b = cast_node<DivideNode>(add_node->get_b()); // DivideNode( SubstractNode(), NumberNode(10) )
  assert(add_a->getValue() == 10);

  auto sub_node = cast_node<SubstractNode>(add_b->get_a());
  auto sub_a = cast_node<NumberNode>(sub_node->get_a());
  auto sub_b = cast_node<NumberNode>(sub_node->get_b());
  assert(sub_a->getValue() == 5);
  assert(sub_b->getValue() == 2);

  auto dem = cast_node<NumberNode>(add_b->get_b());
  assert(dem->getValue() == 6);

  print_success_msg("parser with complex maths expression with parenthesis", 1);
}

void test_complex_maths_expression_without_parenthesis() {
  auto nodes = get_element_nodes_from("10+5-2/6"); // SubstractNode( AddNode(10, 5), DivideNode(2, 6) )
  assert(nodes.size() == 1);

  auto first_element = nodes.front();
  auto sub_node = cast_node<SubstractNode>(first_element);
  auto add_node = cast_node<AddNode>(sub_node->get_a());
  auto div_node = cast_node<DivideNode>(sub_node->get_b());

  auto add_a = cast_node<NumberNode>(add_node->get_a());
  auto add_b = cast_node<NumberNode>(add_node->get_b());
  assert(add_a->getValue() == 10);
  assert(add_b->getValue() == 5);

  auto div_a = cast_node<NumberNode>(div_node->get_a());
  auto div_b = cast_node<NumberNode>(div_node->get_b());
  assert(div_a->getValue() == 2);
  assert(div_b->getValue() == 6);

  print_success_msg("parser with complex maths expression without parenthesis", 1);
}

void test_mutiple_lines() {
  auto nodes = get_element_nodes_from(
    "1+2\n"
    "3-4"
  );
  assert(nodes.size() == 2);

  auto first_node = cast_node<AddNode>(nodes.front());
  auto second_node = cast_node<SubstractNode>(nodes.back());

  assert(cast_node<NumberNode>(first_node->get_a())->getValue() == 1);
  assert(cast_node<NumberNode>(first_node->get_b())->getValue() == 2);
  assert(cast_node<NumberNode>(second_node->get_a())->getValue() == 3);
  assert(cast_node<NumberNode>(second_node->get_b())->getValue() == 4);

  print_success_msg("parser with multiple lines", 1);
}

void test_positions() {
  auto nodes = get_element_nodes_from("5");
  assert(nodes.size() == 1);

  auto node = cast_node<NumberNode>(nodes.front());
  auto pos_start = node->getStartingPosition();
  auto pos_end = node->getEndingPosition();

  assert(pos_start.get_idx() == 0);
  assert(pos_start.get_col() == 0);
  assert(pos_start.get_ln() == 0);

  assert(pos_end.get_idx() == 1);
  assert(pos_end.get_col() == 1);
  assert(pos_end.get_ln() == 0);

  print_success_msg("node's positions are valid", 1);
}

int main() {
  print_title("Parser tests...");

  try {
    test_simple_number();
    test_simple_negative_number();
    test_simple_positive_number();
    test_simple_addition();
    test_simple_substraction();
    test_simple_multiplication();
    test_simple_divison();
    test_simple_modulo();
    test_simple_power();
    test_complex_maths_expression_with_parenthesis();
    test_complex_maths_expression_without_parenthesis();
    test_mutiple_lines();
    test_positions();
  } catch (string cast_exception) {
    cout << "ABORT. Terminating tests due to this error:" << endl;
    cout << cast_exception << endl;
    return 1;
  }

  print_success_msg("All \"Parser\" tests successfully passed");
  return 0;
}
