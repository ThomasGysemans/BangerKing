#include <iostream>
#include "doctest.h"
#include "../include/token.hpp"
#include "../include/position.hpp"
using namespace std;

static const Position pos_start = Position::getDefaultPos();

DOCTEST_TEST_SUITE("Tokens") {
  SCENARIO("tokens") {
    Token token(TokenType::NUMBER, "5", pos_start);
    CHECK(token.getType() == TokenType::NUMBER);
    CHECK(token.getType() != TokenType::DOT);
    CHECK(token.getStringValue() == "5");
    CHECK(token.getStringValue() != "6");
    CHECK(token.ofType(TokenType::NUMBER) == true);
    CHECK(token.ofType(TokenType::STR) == false);
    CHECK(token.ofType(TokenType::STR) == false);
    CHECK(token.notOfType(TokenType::STR) == true);
    CHECK(token.notOfType(TokenType::NUMBER) == false);
  }

  SCENARIO("keyword") {
    Token token(TokenType::KEYWORD, KEYWORDS[0], pos_start);
    CHECK(token.matches(TokenType::KEYWORD, KEYWORDS[0]) == true);
    CHECK(token.matches(TokenType::KEYWORD, KEYWORDS[1]) == false);
  }

  SCENARIO("copy") {
    Token token(TokenType::STR, "hello", pos_start);
    Token copy = token.copy();
    CHECK(&token != &copy);
    CHECK(token.getType() == copy.getType());
    CHECK(token.getStringValue() == copy.getStringValue());
    Position starting_position = token.getStartingPosition();
    Position copy_position = token.getStartingPosition();
    CHECK(&starting_position != &copy_position);
  }

  SCENARIO("pos start") {
    Token token(TokenType::KEYWORD, KEYWORDS[0], pos_start);
    Position token_pos_start = token.getStartingPosition();
    CHECK(&token_pos_start != &pos_start); // Token is supposed to do a copy of the given pos_start
  }

  // if pos_end is not given in the constructor,
  // then pos_end == pos_start
  SCENARIO("implicit pos end") {
    Token token(TokenType::KEYWORD, KEYWORDS[0], pos_start);
    Position start = token.getStartingPosition();
    Position end = token.getEndingPosition();
    CHECK(&start != &end); // "end" is supposed to be a copy
    CHECK(start.equals(end)); // they should have the same the properties though
  }

  SCENARIO("explicit pos end") {
    Position pos_end = Position::getDefaultPos();
    pos_end.advance('h');
    Token token(TokenType::KEYWORD, KEYWORDS[0], pos_start, &pos_end);
    Position token_start = token.getStartingPosition();
    Position token_end = token.getEndingPosition();
    CHECK(&token_start != &pos_end); // it's not meant to be the same
    CHECK(&pos_end != &token_end); // a copy is made
    CHECK(token_end.get_idx() == token_start.get_idx() + 1);
  }
}
