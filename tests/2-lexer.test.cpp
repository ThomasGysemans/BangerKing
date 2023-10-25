#include <iostream>
#include <list>
#include "helper.hpp"
#include "../include/lexer.hpp"
#include "../include/token.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/debug/print_tokens.hpp"
#include "../include/debug/compare_tokens.hpp"
using namespace std;

list<Token*> get_tokens_from(const string& code) {
  Lexer lexer(code);
  return lexer.generate_tokens();
}

std::vector<Token*> list_to_vector(list<Token*> l) {
  return std::vector<Token*> { std::make_move_iterator(l.begin()), std::make_move_iterator(l.end()) };
}

void test_simple_digit() {
  const string code = "5";
  Lexer lexer(code);
  list<Token*> tokens = lexer.generate_tokens();
  assert(tokens.size() == 1);
  assert(tokens.front()->ofType(TokenType::NUMBER));
  assert(tokens.front()->getStringValue() == "5");

  print_success_msg("lexer with simple digits", 1);
}

void test_simple_identifier() {
  const string code = "hello";
  Lexer lexer(code);
  list<Token*> tokens = lexer.generate_tokens();
  assert(tokens.size() == 1);
  assert(tokens.front()->ofType(TokenType::IDENTIFIER));
  assert(tokens.front()->getStringValue() == "hello");

  print_success_msg("lexer with simple identifier", 1);
}

void test_simple_keyword() {
  const string code = KEYWORDS[0];
  Lexer lexer(code);
  list<Token*> tokens = lexer.generate_tokens();
  assert(tokens.size() == 1);
  assert(tokens.front()->ofType(TokenType::KEYWORD));
  assert(tokens.front()->getStringValue() == KEYWORDS[0]);

  print_success_msg("lexer with simple keyword", 1);
}

void test_simple_maths() {
  const string code = "5+6*(6-4/2)%12**42";
  Lexer lexer(code);
  list<Token*> tokens = lexer.generate_tokens();
  list<TokenType> expected_tokens_list = {
    TokenType::NUMBER,
    TokenType::PLUS,
    TokenType::NUMBER,
    TokenType::MULTIPLY,
    TokenType::LPAREN,
    TokenType::NUMBER,
    TokenType::MINUS,
    TokenType::NUMBER,
    TokenType::SLASH,
    TokenType::NUMBER,
    TokenType::RPAREN,
    TokenType::MODULO,
    TokenType::NUMBER,
    TokenType::POWER,
    TokenType::NUMBER
  };

  assert(compare_tokens(expected_tokens_list, tokens));

  print_success_msg("lexer with simple maths", 1);
}

void test_increment() {
  const string code = "--5++";
  Lexer lexer(code);
  list<Token*> tokens = lexer.generate_tokens();
  list<TokenType> expected_tokens_list = {
    TokenType::DEC,
    TokenType::NUMBER,
    TokenType::INC
  };

  assert(compare_tokens(expected_tokens_list, tokens));

  print_success_msg("lexer with simple increment", 1);
}

void test_positions() {
  const string code = "5";
  Lexer lexer(code);
  list<Token*> tokens = lexer.generate_tokens();
  Token* tok = tokens.front();
  Position pos_start = tok->getStartingPosition();
  Position pos_end = tok->getEndingPosition();

  assert(pos_start.get_idx() == 0);
  assert(pos_end.get_idx() == 1);

  print_success_msg("lexer with correct positions", 1);
}

void test_addition_with_whitespace() {
  auto tokens = list_to_vector(get_tokens_from("5 + 5"));
  assert(tokens.size() == 3);
  assert(tokens[0]->getType() == TokenType::NUMBER);
  assert(tokens[1]->getType() == TokenType::PLUS);
  assert(tokens[2]->getType() == TokenType::NUMBER);

  print_success_msg("addition with whitespace", 1);
}

void test_addition_without_whitespace() {
  auto tokens = list_to_vector(get_tokens_from("5+5"));
  assert(tokens.size() == 3);
  assert(tokens[0]->getType() == TokenType::NUMBER);
  assert(tokens[1]->getType() == TokenType::PLUS);
  assert(tokens[2]->getType() == TokenType::NUMBER);

  print_success_msg("addition without whitespace", 1);
}

void test_substraction_with_whitespace() {
  auto tokens = list_to_vector(get_tokens_from("5 - 5"));
  assert(tokens.size() == 3);
  assert(tokens[0]->getType() == TokenType::NUMBER);
  assert(tokens[1]->getType() == TokenType::MINUS);
  assert(tokens[2]->getType() == TokenType::NUMBER);

  print_success_msg("substraction with whitespace", 1);
}

void test_substraction_without_whitespace() {
  auto tokens = list_to_vector(get_tokens_from("5-5"));
  assert(tokens.size() == 3);
  assert(tokens[0]->getType() == TokenType::NUMBER);
  assert(tokens[1]->getType() == TokenType::MINUS);
  assert(tokens[2]->getType() == TokenType::NUMBER);

  print_success_msg("substraction without whitespace", 1);
}

void test_variable_assignment() {
  auto tokens = list_to_vector(get_tokens_from("store a as int = 5"));
  assert(tokens.size() == 6);
  assert(tokens[0]->is_keyword("store"));
  assert(tokens[1]->matches(TokenType::IDENTIFIER, "a"));
  assert(tokens[2]->is_keyword("as"));
  assert(tokens[3]->matches(TokenType::IDENTIFIER, "int"));
  assert(tokens[4]->ofType(TokenType::EQUALS));
  assert(tokens[5]->ofType(TokenType::NUMBER));

  print_success_msg("substraction without whitespace", 1);
}

int main() {
  print_title("Lexer tests...");

  test_simple_digit();
  test_simple_identifier();
  test_simple_keyword();
  test_simple_maths();
  test_increment();
  test_positions();
  test_addition_with_whitespace();
  test_addition_without_whitespace();
  test_substraction_with_whitespace();
  test_substraction_without_whitespace();
  test_variable_assignment();

  print_success_msg("All \"Lexer\" tests successfully passed");
  return 0;
}