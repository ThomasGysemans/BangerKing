#include <iostream>
#include <list>
#include "helper.hpp"
#include "../include/lexer.hpp"
#include "../include/token.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/debug/print_tokens.hpp"
#include "../include/debug/compare_tokens.hpp"
#include "../include/utils/deallocate_list_of_pointers.hpp"
using namespace std;

list<Token*> get_tokens_from(const string& code) {
  Lexer lexer(&code);
  return lexer.generate_tokens();
}

std::vector<Token*> list_to_vector(list<Token*> l) {
  return std::vector<Token*> { std::make_move_iterator(l.begin()), std::make_move_iterator(l.end()) };
}

void test_simple_digit() {
  const string code = "5";
  const list<Token*> tokens = get_tokens_from(code);
  assert(tokens.size() == 1);
  assert(tokens.front()->ofType(TokenType::NUMBER));
  assert(tokens.front()->getStringValue() == "5");

  delete tokens.front();

  print_success_msg("lexer with simple digits", 1);
}

void test_simple_decimal_number() {
  const string code = "3.14";
  const list<Token*> tokens = get_tokens_from(code);
  assert(tokens.size() == 1);
  assert(tokens.front()->ofType(TokenType::NUMBER));
  assert(tokens.front()->getStringValue() == "3.14");

  delete tokens.front();

  print_success_msg("lexer with simple decimal number", 1);
}

void test_simple_identifier() {
  const string code = "hello";
  const list<Token*> tokens = get_tokens_from(code);
  assert(tokens.size() == 1);
  assert(tokens.front()->ofType(TokenType::IDENTIFIER));
  assert(tokens.front()->getStringValue() == "hello");

  delete tokens.front();

  print_success_msg("lexer with simple identifier", 1);
}

void test_simple_keyword() {
  const string code = KEYWORDS[0];
  const list<Token*> tokens = get_tokens_from(code);
  assert(tokens.size() == 1);
  assert(tokens.front()->ofType(TokenType::KEYWORD));
  assert(tokens.front()->getStringValue() == KEYWORDS[0]);

  delete tokens.front();

  print_success_msg("lexer with simple keyword", 1);
}

void test_simple_maths() {
  const string code = "5+6*(6-4/2)%12**42";
  const list<Token*> tokens = get_tokens_from(code);
  const list<TokenType> expected_tokens_list = {
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

  deallocate_const_list_of_pointers<Token>(tokens);

  print_success_msg("lexer with simple maths", 1);
}

void test_increment() {
  const string code = "--5++";
  const list<Token*> tokens = get_tokens_from(code);
  const list<TokenType> expected_tokens_list = {
    TokenType::DEC,
    TokenType::NUMBER,
    TokenType::INC
  };

  assert(compare_tokens(expected_tokens_list, tokens));

  deallocate_const_list_of_pointers<Token>(tokens);

  print_success_msg("lexer with simple increment", 1);
}

void test_positions() {
  const string code = "5";
  const list<Token*> tokens = get_tokens_from(code);
  const Token* tok = tokens.front();
  const Position pos_start = tok->getStartingPosition();
  const Position pos_end = tok->getEndingPosition();

  assert(pos_start.get_idx() == 0);
  assert(pos_end.get_idx() == 1);

  delete tok;

  print_success_msg("lexer with correct positions", 1);
}

void test_operator() {
  const string code = "/";
  const auto tokens = get_tokens_from(code);
  assert(tokens.size() == 1);

  const auto tok = tokens.front();
  assert(tok->getType() == TokenType::SLASH);
  assert(tok->getStartingPosition().get_idx() == 0);
  assert(tok->getEndingPosition().get_idx() == 1);

  delete tok;

  print_success_msg("lexer with math operator", 1);
}

void test_parenthesis() {
  const string code = "(5)";
  const auto tokens = get_tokens_from(code);
  assert(tokens.size() == 3);

  const auto lparen = tokens.front();
  const auto rparen = tokens.back();

  assert(lparen->getType() == TokenType::LPAREN);
  assert(lparen->getStartingPosition().get_idx() == 0); // "(" starts at idx 0 and ends at idx 1
  assert(lparen->getEndingPosition().get_idx() == 1);

  assert(rparen->getType() == TokenType::RPAREN);
  assert(rparen->getStartingPosition().get_idx() == 2); // ")" starts at idx 2 and ends at idx 3
  assert(rparen->getEndingPosition().get_idx() == 3);

  deallocate_const_list_of_pointers<Token>(tokens);

  print_success_msg("lexer with parenthesis", 1);
}

void test_addition_with_whitespace() {
  const auto tokens = list_to_vector(get_tokens_from("5 + 5"));
  assert(tokens.size() == 3);
  assert(tokens[0]->getType() == TokenType::NUMBER);
  assert(tokens[1]->getType() == TokenType::PLUS);
  assert(tokens[2]->getType() == TokenType::NUMBER);

  deallocate_const_vector_of_pointers<Token>(tokens);

  print_success_msg("addition with whitespace", 1);
}

void test_addition_without_whitespace() {
  const auto tokens = list_to_vector(get_tokens_from("5+5"));
  assert(tokens.size() == 3);
  assert(tokens[0]->getType() == TokenType::NUMBER);
  assert(tokens[1]->getType() == TokenType::PLUS);
  assert(tokens[2]->getType() == TokenType::NUMBER);

  deallocate_const_vector_of_pointers<Token>(tokens);

  print_success_msg("addition without whitespace", 1);
}

void test_substraction_with_whitespace() {
  const auto tokens = list_to_vector(get_tokens_from("5 - 5"));
  assert(tokens.size() == 3);
  assert(tokens[0]->getType() == TokenType::NUMBER);
  assert(tokens[1]->getType() == TokenType::MINUS);
  assert(tokens[2]->getType() == TokenType::NUMBER);

  deallocate_const_vector_of_pointers<Token>(tokens);

  print_success_msg("substraction with whitespace", 1);
}

void test_substraction_without_whitespace() {
  const auto tokens = list_to_vector(get_tokens_from("5-5"));
  assert(tokens.size() == 3);
  assert(tokens[0]->getType() == TokenType::NUMBER);
  assert(tokens[1]->getType() == TokenType::MINUS);
  assert(tokens[2]->getType() == TokenType::NUMBER);

  deallocate_const_vector_of_pointers<Token>(tokens);

  print_success_msg("substraction without whitespace", 1);
}

void test_variable_assignment() {
  const auto tokens = list_to_vector(get_tokens_from("store a as int = 5"));
  assert(tokens.size() == 6);
  assert(tokens[0]->is_keyword("store"));
  assert(tokens[1]->matches(TokenType::IDENTIFIER, "a"));
  assert(tokens[2]->is_keyword("as"));
  assert(tokens[3]->matches(TokenType::IDENTIFIER, "int"));
  assert(tokens[4]->ofType(TokenType::EQUALS));
  assert(tokens[5]->ofType(TokenType::NUMBER));

  deallocate_const_vector_of_pointers<Token>(tokens);

  print_success_msg("variable assignment", 1);
}

void test_variable_modification() {
  const auto tokens = list_to_vector(get_tokens_from("a = 5"));
  assert(tokens.size() == 3);
  assert(tokens[0]->matches(TokenType::IDENTIFIER, "a"));
  assert(tokens[1]->ofType(TokenType::EQUALS));
  assert(tokens[2]->ofType(TokenType::NUMBER));

  deallocate_const_vector_of_pointers<Token>(tokens);

  print_success_msg("variable modification", 1);
}

void test_expression_with_identifier() {
  const auto tokens = list_to_vector(get_tokens_from("a+5"));
  assert(tokens.size() == 3);
  assert(tokens[0]->matches(TokenType::IDENTIFIER, "a"));
  assert(tokens[1]->ofType(TokenType::PLUS));
  assert(tokens[2]->ofType(TokenType::NUMBER));

  deallocate_const_vector_of_pointers<Token>(tokens);

  print_success_msg("maths expression with identifier", 1);
}

int main() {
  print_title("Lexer tests...");

  test_simple_digit();
  test_simple_decimal_number();
  test_simple_identifier();
  test_simple_keyword();
  test_simple_maths();
  test_increment();
  test_positions();
  test_operator();
  test_parenthesis();
  test_addition_with_whitespace();
  test_addition_without_whitespace();
  test_substraction_with_whitespace();
  test_substraction_without_whitespace();
  test_variable_assignment();
  test_variable_modification();
  test_expression_with_identifier();

  print_success_msg("All \"Lexer\" tests successfully passed");
  return 0;
}