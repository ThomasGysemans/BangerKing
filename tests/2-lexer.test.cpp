#include <iostream>
#include <list>
#include "doctest.h"
#include "../include/lexer.hpp"
#include "../include/token.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/debug/print_tokens.hpp"
#include "../include/debug/compare_tokens.hpp"
#include "../include/exceptions/illegal_char_error.hpp"
using namespace std;

list<unique_ptr<const Token>> get_tokens_from(const string& code) {
  Lexer lexer(&code);
  READ_FILES.insert({ "<stdin>", make_unique<string>(code) });
  return lexer.generate_tokens();
}

vector<unique_ptr<const Token>> list_to_vector(list<unique_ptr<const Token>> l) {
  return vector<unique_ptr<const Token>> { std::make_move_iterator(l.begin()), std::make_move_iterator(l.end()) };
}

DOCTEST_TEST_SUITE("Lexer") {
  SCENARIO("simple digit") {
    const auto tokens = get_tokens_from("5");
    CHECK(tokens.size() == 1);
    CHECK(tokens.front()->ofType(TokenType::NUMBER));
    CHECK(tokens.front()->getStringValue() == "5");
  }

  SCENARIO("simple decimal number") {
    const auto tokens = get_tokens_from("3.14");
    CHECK(tokens.size() == 1);
    CHECK(tokens.front()->ofType(TokenType::NUMBER));
    CHECK(tokens.front()->getStringValue() == "3.14");
  }

  SCENARIO("simple identifier") {
    const auto tokens = get_tokens_from("hello");
    CHECK(tokens.size() == 1);
    CHECK(tokens.front()->ofType(TokenType::IDENTIFIER));
    CHECK(tokens.front()->getStringValue() == "hello");
  }

  SCENARIO("simple keyword") {
    const auto tokens = get_tokens_from(KEYWORDS[0]);
    CHECK(tokens.size() == 1);
    CHECK(tokens.front()->ofType(TokenType::KEYWORD));
    CHECK(tokens.front()->getStringValue() == KEYWORDS[0]);
  }

  SCENARIO("simple maths") {
    const string code = "5+6*(6-4/2)%12**42";
    const auto tokens = get_tokens_from(code);
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

    CHECK(compare_tokens(expected_tokens_list, tokens));
  }

  SCENARIO("increment") {
    const string code = "--5++";
    const auto tokens = get_tokens_from(code);
    const list<TokenType> expected_tokens_list = {
      TokenType::DEC,
      TokenType::NUMBER,
      TokenType::INC
    };

    CHECK(compare_tokens(expected_tokens_list, tokens));
  }

  SCENARIO("positions") {
    const string code = "5";
    const auto tokens = get_tokens_from(code);
    const Position pos_start = tokens.front()->getStartingPosition();
    const Position pos_end = tokens.front()->getEndingPosition();

    CHECK(pos_start.get_idx() == 0);
    CHECK(pos_end.get_idx() == 1);
  }

  SCENARIO("operator") {
    const string code = "/";
    const auto tokens = get_tokens_from(code);
    CHECK(tokens.size() == 1);

    CHECK(tokens.front()->getType() == TokenType::SLASH);
    CHECK(tokens.front()->getStartingPosition().get_idx() == 0);
    CHECK(tokens.front()->getEndingPosition().get_idx() == 1);
  }

  SCENARIO("parenthesis") {
    const auto tokens = get_tokens_from("(5)");
    CHECK(tokens.size() == 3);

    const auto& lparen = tokens.front();
    const auto& rparen = tokens.back();

    CHECK(lparen->getType() == TokenType::LPAREN);
    CHECK(lparen->getStartingPosition().get_idx() == 0); // "(" starts at idx 0 and ends at idx 1
    CHECK(lparen->getEndingPosition().get_idx() == 1);

    CHECK(rparen->getType() == TokenType::RPAREN);
    CHECK(rparen->getStartingPosition().get_idx() == 2); // ")" starts at idx 2 and ends at idx 3
    CHECK(rparen->getEndingPosition().get_idx() == 3);
  }

  SCENARIO("addition with whitespace") {
    const auto tokens = list_to_vector(get_tokens_from("5 + 5"));
    CHECK(tokens.size() == 3);
    CHECK(tokens[0]->getType() == TokenType::NUMBER);
    CHECK(tokens[1]->getType() == TokenType::PLUS);
    CHECK(tokens[2]->getType() == TokenType::NUMBER);
  }

  SCENARIO("addition without whitespace") {
    const auto tokens = list_to_vector(get_tokens_from("5+5"));
    CHECK(tokens.size() == 3);
    CHECK(tokens[0]->getType() == TokenType::NUMBER);
    CHECK(tokens[1]->getType() == TokenType::PLUS);
    CHECK(tokens[2]->getType() == TokenType::NUMBER);
  }

  SCENARIO("substraction with whitespace") {
    const auto tokens = list_to_vector(get_tokens_from("5 - 5"));
    CHECK(tokens.size() == 3);
    CHECK(tokens[0]->getType() == TokenType::NUMBER);
    CHECK(tokens[1]->getType() == TokenType::MINUS);
    CHECK(tokens[2]->getType() == TokenType::NUMBER);
  }

  SCENARIO("substraction without whitespace") {
    const auto tokens = list_to_vector(get_tokens_from("5-5"));
    CHECK(tokens.size() == 3);
    CHECK(tokens[0]->getType() == TokenType::NUMBER);
    CHECK(tokens[1]->getType() == TokenType::MINUS);
    CHECK(tokens[2]->getType() == TokenType::NUMBER);
  }

  SCENARIO("variable assignment") {
    const auto tokens = list_to_vector(get_tokens_from("store a as int = 5"));
    CHECK(tokens.size() == 6);
    CHECK(tokens[0]->is_keyword("store"));
    CHECK(tokens[1]->matches(TokenType::IDENTIFIER, "a"));
    CHECK(tokens[2]->is_keyword("as"));
    CHECK(tokens[3]->matches(TokenType::IDENTIFIER, "int"));
    CHECK(tokens[4]->ofType(TokenType::EQUALS));
    CHECK(tokens[5]->ofType(TokenType::NUMBER));
  }

  SCENARIO("variable modification") {
    const auto tokens = list_to_vector(get_tokens_from("a = 5"));
    CHECK(tokens.size() == 3);
    CHECK(tokens[0]->matches(TokenType::IDENTIFIER, "a"));
    CHECK(tokens[1]->ofType(TokenType::EQUALS));
    CHECK(tokens[2]->ofType(TokenType::NUMBER));
  }

  SCENARIO("expression with identifier") {
    const auto tokens = list_to_vector(get_tokens_from("a+5"));
    CHECK(tokens.size() == 3);
    CHECK(tokens[0]->matches(TokenType::IDENTIFIER, "a"));
    CHECK(tokens[1]->ofType(TokenType::PLUS));
    CHECK(tokens[2]->ofType(TokenType::NUMBER));
  }

  SCENARIO("string") {
    const auto tokens = list_to_vector(get_tokens_from("\"Hello\" 'yoyo' 'c\\'est' "));
    CHECK(tokens.size() == 3);
    CHECK(tokens[0]->canConcatenate());
    CHECK(tokens[0]->ofType(TokenType::STR));
    CHECK(tokens[0]->getStringValue() == "Hello");

    CHECK(!tokens[1]->canConcatenate());
    CHECK(tokens[1]->ofType(TokenType::STR));
    CHECK(tokens[1]->getStringValue() == "yoyo");
    
    CHECK(!tokens[2]->canConcatenate());
    CHECK(tokens[2]->ofType(TokenType::STR));
    CHECK(tokens[2]->getStringValue() == "c'est");
  }

  SCENARIO("illegal char") {
    CHECK_THROWS_AS(get_tokens_from("é"), IllegalCharError);
  }

  SCENARIO("true and false keywords") {
    const auto tokens = list_to_vector(get_tokens_from("true false"));
    CHECK(tokens.size() == 2);
    CHECK(tokens[0]->ofType(TokenType::KEYWORD));
    CHECK(tokens[0]->is_keyword("true"));
    CHECK(tokens[1]->ofType(TokenType::KEYWORD));
    CHECK(tokens[1]->is_keyword("false"));
  }

  SCENARIO("define") {
    const auto tokens = list_to_vector(get_tokens_from("define PI as double = 3.14"));
    CHECK(tokens.size() == 6);
    CHECK(tokens[0]->ofType(TokenType::KEYWORD));
    CHECK(tokens[1]->ofType(TokenType::IDENTIFIER));
    CHECK(tokens[2]->ofType(TokenType::KEYWORD));
    CHECK(tokens[3]->ofType(TokenType::IDENTIFIER));
    CHECK(tokens[4]->ofType(TokenType::EQUALS));
    CHECK(tokens[5]->ofType(TokenType::NUMBER));
  }

  SCENARIO("boolean operators") {
    const auto tokens = list_to_vector(get_tokens_from("and or not !"));
    CHECK(tokens.size() == 4);
    CHECK(tokens[0]->is_keyword("and"));
    CHECK(tokens[1]->is_keyword("or"));
    CHECK(tokens[2]->is_keyword("not"));
    CHECK(tokens[3]->ofType(TokenType::NOT));
  }
}