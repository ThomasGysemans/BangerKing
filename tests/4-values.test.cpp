#include <iostream>
#include <list>
#include "helper.hpp"
#include "../include/lexer.hpp"
#include "../include/parser.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/values/integer.hpp"
using namespace std;

void test_integer() {
  Lexer lexer("5");
  list<Token*> tokens = lexer.generate_tokens();
  Parser parser(tokens);
  const ListNode* tree = parser.parse();
  list<const CustomNode*> nodes = tree->get_element_nodes();
  const NumberNode* n = cast_node<NumberNode>(nodes.front());
  assert(n->getValue() == 5);

  IntegerValue* integer = new IntegerValue(n->getValue());
  integer->set_pos(n->getStartingPosition(), n->getEndingPosition());
  assert(integer->get_actual_value() == n->getValue());
  assert(integer->get_pos_start()->equals(n->getStartingPosition()));
  assert(integer->get_pos_end()->equals(n->getEndingPosition()));
  auto node_pos_start = n->getStartingPosition();
  assert(integer->get_pos_start() != &node_pos_start);

  delete integer;
  print_success_msg("Integer value", 1);
}

int main() {
  print_title("Values tests...");

  test_integer();

  print_success_msg("All \"Values\" tests successfully passed");

  return 0;
}
