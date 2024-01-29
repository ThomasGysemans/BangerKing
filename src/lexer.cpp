#include "../include/lexer.hpp"
#include "../include/miscellaneous.hpp"
#include "../include/exceptions/illegal_string_error.hpp"
#include "../include/exceptions/illegal_char_error.hpp"
#include "../include/exceptions/unclosed_string_error.hpp"
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

list<unique_ptr<const Token>> Lexer::generate_tokens() {
  list<unique_ptr<const Token>> tokens;
  while (hasMoreTokens()) {
    if (*iter == '\n' || *iter == '\r') {
      const Position pos_start = pos.copy();
      if (*iter == '\r') {
        advance();
      }
      advance();
      tokens.push_back(make_unique<Token>(TokenType::NEWLINE, "\n", pos_start));
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
      tokens.push_back(make_unique<Token>(TokenType::SLASH, "/", pos_start, &pos));
    } else if (*iter == '%') {
      const Position pos_start = pos.copy();
      advance();
      tokens.push_back(make_unique<Token>(TokenType::MODULO, "%", pos_start, &pos));
    } else if (*iter == '(') {
      const Position pos_start = pos.copy();
      advance();
      tokens.push_back(make_unique<Token>(TokenType::LPAREN, "(", pos_start, &pos));
    } else if (*iter == ')') {
      const Position pos_start = pos.copy();
      advance();
      tokens.push_back(make_unique<Token>(TokenType::RPAREN, ")", pos_start, &pos));
    } else if (*iter == '=') {
      const Position pos_start = pos.copy();
      advance();
      tokens.push_back(make_unique<Token>(TokenType::EQUALS, "=", pos_start, &pos));
    } else if (*iter == '!') {
      const Position pos_start = pos.copy();
      advance();
      tokens.push_back(make_unique<Token>(TokenType::NOT, "!", pos_start, &pos)); // TODO: is the string value that necessary?
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

unique_ptr<Token> Lexer::make_identifier() {
  const Position pos_start = pos.copy();
  string identifier = string(1, *iter);
  advance();

  while (hasMoreTokens() && LETTERS_DIGITS.find(*iter) != string::npos) {
    identifier += *iter;
    advance();
  }

  const bool keyword = is_keyword(identifier);
  const TokenType token_type = keyword ? TokenType::KEYWORD : TokenType::IDENTIFIER;
  return make_unique<Token>(token_type, identifier, pos_start, &pos);
}

unique_ptr<Token> Lexer::make_number() {
  const Position pos_start = pos.copy();
  const bool is_beginning_with_dot = *iter == '.';
  string number_str = string(1, *iter);
  int decimal_point_count = 0;
  advance();

  if (is_beginning_with_dot && !string_contains(NORMAL_DIGITS, *iter)) {
    return make_unique<Token>(TokenType::DOT, ".", pos_start);
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
  return make_unique<Token>(TokenType::NUMBER, number_str, pos_start, &pos);
}

unique_ptr<Token> Lexer::make_plus_or_increment() {
  const Position pos_start = pos.copy();
  TokenType tok_type = TokenType::PLUS;
  string value = "+";
  advance();

  if (*iter == '+') {
    advance();
    tok_type = TokenType::INC;
    value = "++";
  }

  return make_unique<Token>(tok_type, value, pos_start, &pos);
}

unique_ptr<Token> Lexer::make_minus_or_decrement() {
  const Position pos_start = pos.copy();
  TokenType tok_type = TokenType::MINUS;
  string value = "-";
  advance();

  if (*iter == '-') {
    advance();
    tok_type = TokenType::DEC;
    value = "--";
  }

  return make_unique<Token>(tok_type, value, pos_start, &pos);
}

unique_ptr<Token> Lexer::make_mul_or_power() {
  const Position pos_start = pos.copy();
  TokenType tok_type = TokenType::MULTIPLY;
  string value = "*";
  advance();

  if (*iter == '*') {
    advance();
    tok_type = TokenType::POWER;
    value = "**";
  }

  return make_unique<Token>(tok_type, value, pos_start, &pos);
}

unique_ptr<Token> Lexer::make_string() {
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
      throw IllegalStringError(
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

  // If the program reached the end of the source code,
  // it means it never encountered the ending quote,
  // meaning that the string was never closed.
  if (!hasMoreTokens()) {
    throw UnclosedStringError(
      pos_start, pos_start,
      "Reached the end of the code without closing this string literal"
    );
  }

  advance(); // to skip the ending quote (the lexer must not believe it's the start of a new string).

  return make_unique<Token>(TokenType::STR, value, pos_start, &pos, allow_concatenation);
}