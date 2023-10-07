#include <iostream>
#include <list>
#include "helper.h"
#include "../include/token.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/nodes.h"
#include "../include/miscellaneous.h"
using namespace std;

static list<const CustomNode*> get_element_nodes_from(const string& code, bool debug_print = false) {
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

/// @brief Casts an instance of CustomNode into a derived class of CustomNode. Note that it will throw an error if the cast isn't impossible.
/// @tparam T The expected type of the derived class.
/// @param b The instance of CustomNode.
/// @return The instance of the derived class `T` from `b`
template <typename T>
static const T* cast_node(const CustomNode* b) {
  if (!instanceof<T>(b)) {
    throw string("incorrect cast of CustomNode");
  }
  return dynamic_cast<const T*>(b);
}

void test_simple_number() {
  auto element_nodes = get_element_nodes_from("5");
  auto first_element = element_nodes.front();
  auto number_node = cast_node<NumberNode>(first_element);

  assert(element_nodes.size() == 1);
  assert(number_node->getValue() == 5);

  cout << get_success_msg("parser with simple number", 1) << endl;
}

void test_simple_negative_number() {
  auto nodes = get_element_nodes_from("-5");
  auto first_element = nodes.front();
  auto minus_node = cast_node<MinusNode>(first_element);
  auto number_node = cast_node<NumberNode>(minus_node->get_node());
  assert(nodes.size() == 1);
  assert(number_node->getValue() == 5);

  cout << get_success_msg("parser with simple negative number", 1) << endl;
}

void test_simple_positive_number() {
  auto nodes = get_element_nodes_from("+670");
  auto first_element = nodes.front();
  auto plus_node = cast_node<PlusNode>(first_element);
  auto number_node = cast_node<NumberNode>(plus_node->get_node());
  assert(nodes.size() == 1);
  assert(number_node->getValue() == 670);

  cout << get_success_msg("parser with simple positive number", 1) << endl;
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

  cout << get_success_msg("parser with simple addition", 1) << endl;
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

  cout << get_success_msg("parser with simple substraction", 1) << endl;
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

  cout << get_success_msg("parser with simple multiplication", 1) << endl;
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

  cout << get_success_msg("parser with simple divison", 1) << endl;
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

  cout << get_success_msg("parser with simple modulo", 1) << endl;
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

  cout << get_success_msg("parser with simple power", 1) << endl;
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

  cout << get_success_msg("parser with complex maths expression with parenthesis", 1) << endl;
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

  cout << get_success_msg("parser with complex maths expression without parenthesis", 1) << endl;
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

  cout << get_success_msg("parser with multiple lines", 1) << endl;
}

int main() {
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
  } catch (string cast_exception) {
    cout << "ABORT. Terminating tests due to this error:" << endl;
    cout << cast_exception << endl;
    return 1;
  }

  cout << get_success_msg("All \"Parser\" tests successfully passed") << endl;
  return 0;
}
