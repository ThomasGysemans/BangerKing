#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <map>
#include <list>
#include "position.h"
#include "token.h"
using namespace std;

static const string NORMAL_DIGITS = "0123456789";
static const string DIGITS = NORMAL_DIGITS + "_";
static const string LETTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const string LETTERS_UNDERSCORE = LETTERS + "_";
static const string LETTERS_DIGITS = LETTERS + DIGITS;
static const string LETTERS_DIGITS_UNDERSCORE = LETTERS_DIGITS + "_";
static const map<char, char> ESCAPE_CHARACTERS{{'n', '\n'}, {'t', '\t'}, {'r', '\r'}};

class Lexer {
  const string filename;
  const string* text;
  string::const_iterator iter;
  Position pos;

  void advance() {
    ++iter;
    pos.advance(*iter);
  }

  /// @brief Did we reach the end of the source code?
  /// @return `true` if there is still some code to read
  bool hasMoreTokens() const {
    return iter != (*text).end();
  }

  public:
    /// @brief Creates an instance of lexer.
    /// @param t The source code
    /// @param fi The filename
    Lexer(
      const string& t,
      const string fi = "<stdin>"
    ): text(&t), filename(fi), pos(Position(-1, 0, -1, fi)) {
      iter = t.begin();
    }

    /// @brief Generates the tokens in a list.
    /// @return The list of tokens in the given source code.
    list<Token*> generate_tokens() {
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

  private:
    /// @brief Makes an identifier.(a simple word starting with a letter or an underscore)
    /// @return A token that is either a keyword or an identifier.
    Token * make_identifier() {
      const Position pos_start = pos.copy();
      string identifier = string(1, *iter);
      advance();

      while (hasMoreTokens() && LETTERS_DIGITS_UNDERSCORE.find(*iter) != string::npos) {
        identifier += *iter;
        advance();
      }

      const bool is_keyword = list_contains(KEYWORDS, identifier);
      const TokenType token_type = is_keyword ? TokenType::KEYWORD : TokenType::IDENTIFIER;
      return new Token(token_type, identifier, pos_start, pos);
    }

    /// @brief Makes a number or a DOT.
    /// @return A token for a number (integer as well as float and double), or a dot.
    Token * make_number() {
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
      const bool is_int = string_contains(number_str, '.');
      return new Token(TokenType::NUMBER, number_str, pos_start, pos);
    }

    /// @brief Makes a token of type + or ++
    /// @return A token of type PLUS or INC
    Token * make_plus_or_increment() {
      const Position pos_start = pos.copy();
      TokenType tok_type = TokenType::PLUS;
      string value = "+";
      advance();

      if (*iter == '+') {
        advance();
        tok_type = TokenType::INC;
        value = "++";
      }

      return new Token(tok_type, value, pos_start, pos);
    }

    /// @brief Makes a token of type - or --
    /// @return A token of type MINUS or DEC
    Token * make_minus_or_decrement() {
      const Position pos_start = pos.copy();
      TokenType tok_type = TokenType::MINUS;
      string value = "-";
      advance();

      if (*iter == '-') {
        advance();
        tok_type = TokenType::DEC;
        value = "--";
      }

      return new Token(tok_type, value, pos_start, pos);
    }

    /// @brief Makes a token of type * or **
    /// @return A token of type MUL or POWER
    Token * make_mul_or_power() {
      const Position pos_start = pos.copy();
      TokenType tok_type = TokenType::MULTIPLY;
      string value = "*";
      advance();

      if (*iter == '*') {
        advance();
        tok_type = TokenType::POWER;
        value = "**";
      }

      return new Token(tok_type, value, pos_start, pos);
    }
};

#endif