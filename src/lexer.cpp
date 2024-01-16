#include "../include/lexer.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/exceptions/type_overflow_error.hpp"
#include "../include/exceptions/illegal_char_error.hpp"
using namespace std;

const string NORMAL_DIGITS = "0123456789";
const string DIGITS = NORMAL_DIGITS + "_";
const string LETTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const string LETTERS_UNDERSCORE = LETTERS + "_";
const string LETTERS_DIGITS = LETTERS + DIGITS;
const map<char, char> ESCAPE_CHARACTERS{{'n', '\n'}, {'t', '\t'}, {'r', '\r'}};
const char DOUBLE_QUOTE = '"';
const char SIMPLE_QUOTE = '\'';
const char BACKSLASH = '\\';

/// @brief Helper method to know if a specific identifier is a keyword or not.
/// @param keyword The identifier that may or may not be a keyword.
/// @return `true` if the given identifier is indeed a keyword.
bool is_keyword(const string& keyword) {
  return std::find(KEYWORDS.begin(), KEYWORDS.end(), keyword) != KEYWORDS.end();
}

Lexer::Lexer(
  const string* t,
  const string filename
): text(t), filename(filename), pos(Position(0, 0, 0, filename)) {
  iter = t->begin();
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
    } else if (*iter == '.' || string_contains(DIGITS, *iter)) { // numbers are allowed to start with a dot (in case they're >= 0 and < 1)
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
      tokens.push_back(new Token(TokenType::SLASH, "/", pos_start, &pos));
    } else if (*iter == '%') {
      const Position pos_start = pos.copy();
      advance();
      tokens.push_back(new Token(TokenType::MODULO, "%", pos_start, &pos));
    } else if (*iter == '(') {
      const Position pos_start = pos.copy();
      advance();
      tokens.push_back(new Token(TokenType::LPAREN, "(", pos_start, &pos));
    } else if (*iter == ')') {
      const Position pos_start = pos.copy();
      advance();
      tokens.push_back(new Token(TokenType::RPAREN, ")", pos_start, &pos));
    } else if (*iter == '=') {
      const Position pos_start = pos.copy();
      advance();
      tokens.push_back(new Token(TokenType::EQUALS, "=", pos_start, &pos));
    } else if (*iter == DOUBLE_QUOTE || *iter == SIMPLE_QUOTE) {
      tokens.push_back(make_string());
    } else {
      if (*iter == ' ') {
        advance();
      } else {
        throw IllegalCharError(
          pos, pos,
          string(1, *iter)
        );
      }
    }
  }
  return tokens;
}

/*
*
* Makers
* Those methods are responsible of constructing complex types of tokens (such as numbers, identifiers, keywords, etc.)
* They are tokens composed of multiple characters.
*
*/

Token* Lexer::make_identifier() {
  const Position pos_start = pos.copy();
  string identifier = string(1, *iter);
  advance();

  while (hasMoreTokens() && LETTERS_DIGITS.find(*iter) != string::npos) {
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

Token* Lexer::make_string() {
  const Position pos_start = pos.copy();
  char opening_quote = (*iter);
  bool allow_concatenation = *iter == DOUBLE_QUOTE;
  string value = "";
  advance();

  bool escaped = false; // `true` if the previous character was a backslash (\)
  while (
    hasMoreTokens() &&
    (escaped || *iter != opening_quote)
  ) {
    if (value.length() == UINT_MAX) {
      throw TypeOverflowError(
        pos_start, pos,
        "The maximum length of a string has been reached: " + std::to_string(value.length())
      );
    }
    if (*iter == BACKSLASH) {
      if (escaped) {
        value.push_back(BACKSLASH);
        escaped = false;
      } else {
        escaped = true;
      }
    } else {
      value.push_back(*iter);
      escaped = false;
    }
    advance();
  }

  advance(); // to skip the ending quote (the lexer must not believe it's the start of a new string).

  return new Token(TokenType::STR, value, pos_start, &pos, allow_concatenation);
}