#include <iostream>
#include <list>
#include "helper.h"
#include "../include/lexer.h"
#include "../include/token.h"
#include "../include/miscellaneous.h"
#include "../include/debug/print_tokens.h"
#include "../include/debug/compare_tokens.h"
using namespace std;

void test_simple_digit() {
  const string code = "5";
  Lexer lexer(code);
  list<Token*> tokens = lexer.generate_tokens();
  assert(tokens.size() == 1);
  assert(tokens.front()->ofType(TokenType::NUMBER));
  assert(tokens.front()->getStringValue() == "5");

  cout << get_success_msg("lexer with simple digits", 1) << endl;
}

void test_simple_identifier() {
  const string code = "hello";
  Lexer lexer(code);
  list<Token*> tokens = lexer.generate_tokens();
  assert(tokens.size() == 1);
  assert(tokens.front()->ofType(TokenType::IDENTIFIER));
  assert(tokens.front()->getStringValue() == "hello");

  cout << get_success_msg("lexer with simple identifier", 1) << endl;
}

void test_simple_keyword() {
  const string code = KEYWORDS[0];
  Lexer lexer(code);
  list<Token*> tokens = lexer.generate_tokens();
  assert(tokens.size() == 1);
  assert(tokens.front()->ofType(TokenType::KEYWORD));
  assert(tokens.front()->getStringValue() == KEYWORDS[0]);

  cout << get_success_msg("lexer with simple keyword", 1) << endl;
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

  cout << get_success_msg("lexer with simple maths", 1) << endl;
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
}

int main() {
  test_simple_digit();
  test_simple_identifier();
  test_simple_keyword();
  test_simple_maths();
  test_increment();

  cout << get_success_msg("All \"Lexer\" tests successfully passed") << endl;
  return 0;
}