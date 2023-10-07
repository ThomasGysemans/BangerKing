#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include "position.h"
#include "miscellaneous.h"

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

enum Type {
  INT,
  DOUBLE,
  STRING,
  BOOLEAN,
};

const string KEYWORDS[] = {
  "store",
  "as",
  "and",
  "or",
  "not",
  "feature",
  "return",
  "creating",
  "int",
  "double",
  "void", // might be a type, not sure yet
};

class Token {
  const TokenType type;
  const string value;
  const Position pos_start;
  const Position pos_end;
  const bool allow_concatenation;

  public:
    Token(
      const TokenType& t,
      const string& v,
      const Position& start,
      const Position& end = NullPosition(),
      const bool concatenation = false
    ):
      type(t),
      value(v),
      allow_concatenation(concatenation),
      pos_start(start.copy()),
      pos_end(instanceof<NullPosition>(&end) ? start.copy() : end.copy()) { }
    
    ~Token() { }

    /**
     * Checks if a the type and the value of a token correspond.
     * @param type The type of token (`TokenType.KEYWORD` for example).
     * @param value The value that has to correspond.
     * @returns `true` if the type & value of a token correspond with `type` and `value`.
     */
    bool matches(const TokenType& type, const string& value) const {
      return this->type == type && this->value == value;
    }

    TokenType getType() const { return type; }
    Position getStartingPosition() const { return pos_start; }
    Position getEndingPosition() const { return pos_end; }
    string getStringValue() const { return value; }

    /**
     * Checks if the type of this token matches the given type.
     * @param type The TokenType to test.
     * @returns `true` if this token is of type `type`.
     */
    bool ofType(const TokenType& type) const {
      return this->type == type;
    }

    /**
     * Checks if the type of this token doesn't match the given type.
     * @param type The TokenType to test.
     * @returns `true` if this token is not of type `type`.
     */
    bool notOfType(const TokenType& type) const {
      return !this->ofType(type);
    }

    /**
     * Creates a deep copy of this instance, a clone.
     * @returns A new instance of Token with the same data.
     */
    Token copy() const {
      return Token(*this);
    }
};

#endif