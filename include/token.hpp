#pragma once

#include <vector>
#include "position.hpp"
#include "miscellaneous.hpp"

using namespace std;

enum TokenType {
  NUMBER,
  PLUS,
  MINUS,
  INC,
  DEC,
  MULTIPLY,
  SLASH,
  POWER,
  MODULO,
  LPAREN,
  RPAREN,
  IDENTIFIER,
  KEYWORD,
  EQUALS,
  DOUBLE_EQUALS,
  NOT_EQUAL,
  LT,
  GT,
  LTE,
  GTE,
  COLON,
  NEWLINE,
  LSQUARE,
  RSQUARE,
  LBRACK,
  RBRACK,
  COMMA,
  STR,
  DOT,
  LCHEVRON,
  RCHEVRON,
  HASH
};

extern const vector<string> KEYWORDS;

class Token {
  const TokenType type;
  const string value;
  const bool allow_concatenation;
  const Position pos_start;
  const Position pos_end;

  public:
    Token(
      const TokenType& t,
      const string& v,
      const Position& start,
      const Position* end = nullptr,
      const bool concatenation = false
    );

    /// @brief Checks if a the type and the value of a token correspond.
    /// @param type The type of token (`TokenType.KEYWORD` for example).
    /// @param value The value that has to correspond.
    /// @return `true` if the type & value of a token correspond with `type` and `value`.
    bool matches(const TokenType& type, const string& value) const;

    /// @brief Tests if this token is a keyword and if it corresponds to the expected value.
    /// @param expected_value The value that the token should have, if it's indeed a keyword.
    /// @return `true` if this token is a keyword and matches the expected value.
    bool is_keyword(const string& expected_value) const;

    /// @brief Gets the type of the token.
    /// @return An instance of the `TokenType` enum
    TokenType getType() const;

    // TODO: these getters should return a reference instead of a copy

    /// @brief Gets the starting position of the token.
    /// @return The copy of the starting position of the token.
    Position getStartingPosition() const;

    /// @brief Gets the ending position of the token.
    /// @return The copy of the ending position of the token.
    Position getEndingPosition() const;

    /// @brief Gets the value of the token as a string.
    /// @return A string that represents the value that this token holds.
    string getStringValue() const;

    /// @brief Checks if the type of this token matches the given type.
    /// @param type The TokenType to test.
    /// @return `true` if this token is of type `type`.
    bool ofType(const TokenType& type) const;

    /// @brief Checks if the type of this token doesn't match the given type.
    /// @param type The TokenType to test.
    /// @return `true` if this token is not of type `type`.
    bool notOfType(const TokenType& type) const;

    /// @brief If the token is a string, can it include variables for concatenation?
    /// @return `true` if `allow_concatenation` is `true`, `false` otherwise.
    bool canConcatenate() const;

    /// @brief Creates a deep copy of this instance, a clone.
    /// @return A new instance of Token with the same data.
    Token copy() const;
};
