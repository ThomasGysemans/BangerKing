#include <iostream>
#include "helper.hpp"
#include "../include/lexer.hpp"
#include "../include/token.hpp"
#include "../include/position.hpp"
using namespace std;

void test_position() {
  Position pos = Position::getDefaultPos();
  assert(pos.get_idx() == 0);
  assert(pos.get_col() == 0);
  assert(pos.get_ln() == 0);
  assert(pos.get_filename() == "<hidden>");

  print_success_msg("default position", 1);
}

void test_advanced_position() {
  Position pos = Position::getDefaultPos();
  pos.advance('5');
  assert(pos.get_idx() == 1);
  assert(pos.get_col() == 1);
  assert(pos.get_ln() == 0);

  print_success_msg("advanced position on a single line", 1);
}

void test_multiline_position() {
  Position pos = Position::getDefaultPos();
  pos.advance('5');
  pos.advance('\n');
  assert(pos.get_idx() == 2);
  assert(pos.get_col() == 0);
  assert(pos.get_ln() == 1);
  
  print_success_msg("advanced position on multiple lines", 1);
}

void test_to_string() {
  Position pos = Position::getDefaultPos();
  pos.advance('6');
  pos.advance('0');
  pos.advance('\n');

  string str = pos.to_string();
  assert(str == "1:0, idx=3");

  print_success_msg("position as string", 1);
}

int main() {
  print_title("Positions tests...");

  test_position();
  test_advanced_position();
  test_multiline_position();
  test_to_string();

  print_success_msg("All \"Positions\" tests successfully passed");
  return 0;
}