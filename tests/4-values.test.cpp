#include <iostream>
#include <list>
#include "helper.hpp"
#include "../include/lexer.hpp"
#include "../include/parser.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/values/integer.hpp"
#include "../include/utils/deallocate_list_of_pointers.hpp"
using namespace std;

void test_integer() {
  const string code = "5";
  Lexer lexer(&code);
  list<Token*> tokens = lexer.generate_tokens();
  Parser parser(tokens);
  const ListNode* tree = parser.parse();
  const list<const CustomNode*> nodes = tree->get_element_nodes();
  const IntegerNode* n = cast_node<IntegerNode>(nodes.front());
  assert(n->getValue() == 5);
  assert(n->getStartingPosition().get_idx() == 0);
  assert(n->getEndingPosition().get_idx() == 1);

  IntegerValue* integer = new IntegerValue(n->getValue());
  integer->set_pos(n->getStartingPosition(), n->getEndingPosition());
  assert(integer->get_actual_value() == n->getValue());
  assert(integer->get_pos_start()->equals(n->getStartingPosition()));
  assert(integer->get_pos_end()->equals(n->getEndingPosition()));
  
  const auto node_pos_start = n->getStartingPosition();
  assert(integer->get_pos_start() != &node_pos_start);

  delete integer;
  delete tree;
  deallocate_list_of_pointers<Token>(tokens);

  print_success_msg("integer value", 1);
}

void test_copy() {
  const IntegerValue* integer = new IntegerValue(5);
  const IntegerValue* copy = integer->copy();

  const Position* copy_pos_start = copy->get_pos_start();
  const Position* copy_pos_end = copy->get_pos_end();

  assert(integer->get_actual_value() == copy->get_actual_value());
  assert(integer->get_pos_start() != copy_pos_start);
  assert(integer->get_pos_end() != copy_pos_end);
  assert(integer->get_pos_start()->equals(*copy_pos_start));
  assert(integer->get_pos_end()->equals(*copy_pos_end));
  assert(&integer != &copy);

  delete integer;
  delete copy;
  print_success_msg("copy of an integer", 1);
}

int main() {
  print_title("Values tests...");

  test_integer();
  test_copy();

  print_success_msg("All \"Values\" tests successfully passed");

  return 0;
}
