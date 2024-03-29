#pragma once

#include <vector>
#include <string>
#include "position.hpp"

enum TokenType {
  NUMBER, // integer or double
  PLUS, // +
  MINUS, // -
  INC, // ++
  DEC, // --
  MULTIPLY, // *
  SLASH, // /
  POWER, // **
  MODULO, // %
  LPAREN, // (
  RPAREN, // )
  IDENTIFIER, // a simple word
  KEYWORD, // a simple word contained in "KEYWORDS"
  EQUALS, // =
  DOUBLE_EQUALS, // =
  NOT_EQUAL, // !=
  LT, // <
  GT, // >
  LTE, // <=
  GTE, // >=
  NOT, // !
  COLON, // :
  NEWLINE, // \n
  LSQUARE, // [
  RSQUARE, // ]
  LBRACK, // {
  RBRACK, // }
  COMMA, // ,
  STR, // a string
  DOT, // .
  HASH // #
};

extern const std::vector<std::string> KEYWORDS;

class Token {
  const TokenType type;
  const std::string value;
  const bool allow_concatenation;
  const Position pos_start;
  const Position pos_end;

  public:
    Token(
      const TokenType& t,
      std::string v,
      const Position& start,
      const Position* end = nullptr,
      bool concatenation = false
    );

    /// @brief Checks if a the type and the value of a token correspond.
    /// @param type The type of token (`TokenType.KEYWORD` for example).
    /// @param value The value that has to correspond.
    /// @return `true` if the type & value of a token correspond with `type` and `value`.
    [[nodiscard]] bool matches(const TokenType& type, const std::string& value) const;

    /// @brief Tests if this token is a keyword and if it corresponds to the expected value.
    /// @param expected_value The value that the token should have, if it's indeed a keyword.
    /// @return `true` if this token is a keyword and matches the expected value.
    [[nodiscard]] bool is_keyword(const std::string& expected_value) const;

    /// @brief Checks if this token's string value is the same as the given string value.
    /// @param string_value The string value with which to compare this token.
    /// @return `true` if the string value of this token is equal to the given string value.
    [[nodiscard]] bool is(const std::string& string_value) const;

    /// @brief Gets the type of the token.
    /// @return An instance of the `TokenType` enum
    [[nodiscard]] TokenType getType() const;

    /// @brief Gets the starting position of the token.
    /// @return The copy of the starting position of the token.
    [[nodiscard]] Position getStartingPosition() const;

    /// @brief Gets the ending position of the token.
    /// @return The copy of the ending position of the token.
    [[nodiscard]] Position getEndingPosition() const;

    /// @brief Gets the value of the token as a string.
    /// @return A string that represents the value that this token holds.
    [[nodiscard]] std::string getStringValue() const;

    /// @brief Checks if the type of this token matches the given type.
    /// @param type The TokenType to test.
    /// @return `true` if this token is of type `type`.
    [[nodiscard]] bool ofType(const TokenType& type) const;

    /// @brief Checks if the type of this token doesn't match the given type.
    /// @param type The TokenType to test.
    /// @return `true` if this token is not of type `type`.
    [[nodiscard]] bool notOfType(const TokenType& type) const;

    /// @brief If the token is a string, can it include variables for concatenation?
    /// @return `true` if `allow_concatenation` is `true`, `false` otherwise.
    [[nodiscard]] bool canConcatenate() const;

    /// @brief Creates a deep copy of this instance, a clone.
    /// @return A new instance of Token with the same data.
    [[nodiscard]] Token copy() const;
};
