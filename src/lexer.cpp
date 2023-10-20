#include "../include/lexer.hpp"
#include "../include/miscellaneous.hpp"

#include <iostream>

using namespace std;

const string NORMAL_DIGITS = "0123456789";
const string DIGITS = NORMAL_DIGITS + "_";
const string LETTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const string LETTERS_UNDERSCORE = LETTERS + "_";
const string LETTERS_DIGITS = LETTERS + DIGITS;
const string LETTERS_DIGITS_UNDERSCORE = LETTERS_DIGITS + "_";
const map<char, char> ESCAPE_CHARACTERS{{'n', '\n'}, {'t', '\t'}, {'r', '\r'}};

bool is_keyword(const string& keyword) {
  return std::find(KEYWORDS.begin(), KEYWORDS.end(), keyword) != KEYWORDS.end();
}

Lexer::Lexer(
  const string& t,
  const string filename
): text(&t), filename(filename), pos(Position(0, 0, 0, filename)) {
  iter = t.begin();
}

void Lexer::advance() {
  ++iter;
  pos.advance(*iter);
}

bool Lexer::hasMoreTokens() const {
  return iter != (*text).end();
}

list<Token*> Lexer::generate_tokens() {
  list<Token*> tokens;
  while (hasMoreTokens()) {
    if (*iter == '\n' || *iter == '\r') {
      const Position pos_start = pos.copy();
      if (*iter == '\r') {
        advance();
      }
      advance();
      tokens.push_back(new Token(TokenType::NEWLINE, "\n", pos_start));
    } else if (string_contains(LETTERS_UNDERSCORE, *iter)) { // must be before "make_number()"
      tokens.push_back(make_identifier());
    } else if (*iter == '.' || string_contains(DIGITS, *iter)) {
      tokens.push_back(make_number());
    } else if (*iter == '+') {
      tokens.push_back(make_plus_or_increment());
    } else if (*iter == '-') {
      tokens.push_back(make_minus_or_decrement());
    } else if (*iter == '*') {
      tokens.push_back(make_mul_or_power());
    } else if (*iter == '/') {
      const Position pos_start = pos.copy();
      advance();
      tokens.push_back(new Token(TokenType::SLASH, "/", pos_start));
    } else if (*iter == '%') {
      const Position pos_start = pos.copy();
      advance();
      tokens.push_back(new Token(TokenType::MODULO, "%", pos_start));
    } else if (*iter == '(') {
      const Position pos_start = pos.copy();
      advance();
      tokens.push_back(new Token(TokenType::LPAREN, "(", pos));
    } else if (*iter == ')') {
      const Position pos_start = pos.copy();
      advance();
      tokens.push_back(new Token(TokenType::RPAREN, ")", pos));
    } else {
      advance();
    }
  }
  return tokens;
}

/*
*
* Makers
*
*/

Token* Lexer::make_identifier() {
  const Position pos_start = pos.copy();
  string identifier = string(1, *iter);
  advance();

  while (hasMoreTokens() && LETTERS_DIGITS_UNDERSCORE.find(*iter) != string::npos) {
    identifier += *iter;
    advance();
  }

  const bool keyword = is_keyword(identifier);
  const TokenType token_type = keyword ? TokenType::KEYWORD : TokenType::IDENTIFIER;
  return new Token(token_type, identifier, pos_start, &pos);
}

Token* Lexer::make_number() {
  const Position pos_start = pos.copy();
  const bool is_beginning_with_dot = *iter == '.';
  string number_str = string(1, *iter);
  int decimal_point_count = 0;
  advance();

  if (is_beginning_with_dot && !string_contains(NORMAL_DIGITS, *iter)) {
    return new Token(TokenType::DOT, ".", pos_start);
  }

  const string digits_and_point = DIGITS + ".";
  while (hasMoreTokens() && string_contains(digits_and_point, *iter)) {
    if (*iter == '.') {
      ++decimal_point_count;
      if (decimal_point_count > 1) {
        break;
      }
    }

    number_str += *iter;
    advance();
  }

  if (number_str.starts_with('.')) {
    number_str = '0' + number_str;
  } else if (number_str.ends_with('.')) {
    number_str += '0';
  }

  remove_character(number_str, '_');
  // const bool is_int = string_contains(number_str, '.'); // TODO: needed to later separate "integer" from "double"
  return new Token(TokenType::NUMBER, number_str, pos_start, &pos);
}

Token* Lexer::make_plus_or_increment() {
  const Position pos_start = pos.copy();
  TokenType tok_type = TokenType::PLUS;
  string value = "+";
  advance();

  if (*iter == '+') {
    advance();
    tok_type = TokenType::INC;
    value = "++";
  }

  return new Token(tok_type, value, pos_start, &pos);
}

Token* Lexer::make_minus_or_decrement() {
  const Position pos_start = pos.copy();
  TokenType tok_type = TokenType::MINUS;
  string value = "-";
  advance();

  if (*iter == '-') {
    advance();
    tok_type = TokenType::DEC;
    value = "--";
  }

  return new Token(tok_type, value, pos_start, &pos);
}

Token* Lexer::make_mul_or_power() {
  const Position pos_start = pos.copy();
  TokenType tok_type = TokenType::MULTIPLY;
  string value = "*";
  advance();

  if (*iter == '*') {
    advance();
    tok_type = TokenType::POWER;
    value = "**";
  }

  return new Token(tok_type, value, pos_start, &pos);
}