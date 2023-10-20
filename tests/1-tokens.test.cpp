#include <iostream>
#include "helper.hpp"
#include "../include/token.hpp"
#include "../include/position.hpp"
using namespace std;

static const Position pos_start = Position::getDefaultPos();

void test_tokens() {
  Token token(TokenType::NUMBER, "5", pos_start);
  assert(token.getType() == TokenType::NUMBER);
  assert(token.getType() != TokenType::DOT);
  assert(token.getStringValue() == "5");
  assert(token.getStringValue() != "6");
  assert(token.ofType(TokenType::NUMBER) == true);
  assert(token.ofType(TokenType::STR) == false);
  assert(token.ofType(TokenType::STR) == false);
  assert(token.notOfType(TokenType::STR) == true);
  assert(token.notOfType(TokenType::NUMBER) == false);
  print_success_msg("The methods of the Token class are good.", 1);
}

void test_keyword() {
  Token token(TokenType::KEYWORD, KEYWORDS[0], pos_start);
  assert(token.matches(TokenType::KEYWORD, KEYWORDS[0]) == true);
  assert(token.matches(TokenType::KEYWORD, KEYWORDS[1]) == false);
  print_success_msg("using keywords and comparing them work too", 1);
}

void test_copy() {
  Token token(TokenType::STR, "hello", pos_start);
  Token copy = token.copy();
  assert(&token == &copy == false);
  assert(&token == &token);
  assert(token.getType() == copy.getType());
  assert(token.getStringValue() == copy.getStringValue());
  Position starting_position = token.getStartingPosition();
  Position copy_position = token.getStartingPosition();
  assert(&starting_position == &copy_position == false);
  print_success_msg("copying tokens works!", 1);
}

void test_pos_start() {
  Token token(TokenType::KEYWORD, KEYWORDS[0], pos_start);
  Position token_pos_start = token.getStartingPosition();
  assert(&token_pos_start == &pos_start == false); // Token is supposed to do a copy of the given pos_start
  print_success_msg("copying pos_start works", 1);
}

// if pos_end is not given in the constructor,
// then pos_end == pos_start
void test_implicit_pos_end() {
  Token token(TokenType::KEYWORD, KEYWORDS[0], pos_start);
  Position start = token.getStartingPosition();
  Position end = token.getEndingPosition();
  assert(&start == &end == false); // "end" is supposed to be a copy
  assert(start.get_ln() == end.get_ln());
  assert(start.get_col() == end.get_col());
  assert(start.get_idx() == end.get_idx());
  assert(start.get_filename() == end.get_filename());
  print_success_msg("implicit pos_end works", 1);
}

void test_explicit_pos_end() {
  Position pos_end = Position::getDefaultPos();
  pos_end.advance('h');
  Token token(TokenType::KEYWORD, KEYWORDS[0], pos_start, &pos_end);
  Position token_start = token.getStartingPosition();
  Position token_end = token.getEndingPosition();
  assert(&token_start == &pos_end == false); // it's not meant to be the same
  assert(&pos_end == &token_end == false); // a copy is supposed to be made
  assert(token_end.get_idx() == token_start.get_idx() + 1);
  print_success_msg("explicit pos_end works", 1);
}

int main() {
  print_title("Tokens tests...");

  test_tokens();
  test_keyword();
  test_copy();
  test_pos_start();
  test_implicit_pos_end();
  test_explicit_pos_end();

  print_success_msg("All \"Tokens\" tests successfully passed");
  return 0;
}