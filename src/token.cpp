#include "../include/token.hpp"
using namespace std;

const vector<string> KEYWORDS = {
  "store",
  "true",
  "false",
  "as",
  "and",
  "or",
  "not",
  "define",
  "feature",
  "return",
  "creating",
  "void"
};

Token::Token(
  const TokenType& t,
  string v,
  const Position& start,
  const Position* end,
  const bool concatenation
):
  type(t),
  value(move(v)),
  allow_concatenation(concatenation),
  pos_start(start.copy()),
  pos_end(end == nullptr ? start.copy() : end->copy()) {}

bool Token::matches(const TokenType& type, const string& value) const {
  return this->type == type && this->value == value;
}

bool Token::is_keyword(const string& expected_value) const {
  return matches(TokenType::KEYWORD, expected_value);
}

bool Token::is(const string& string_value) const {
  return this->value == string_value;
}

TokenType Token::getType() const { return type; }
Position Token::getStartingPosition() const { return pos_start; }
Position Token::getEndingPosition() const { return pos_end; }
string Token::getStringValue() const { return value; }

bool Token::ofType(const TokenType& type) const { return this->type == type; }
bool Token::notOfType(const TokenType& type) const { return !ofType(type); }
bool Token::canConcatenate() const { return allow_concatenation; }
Token Token::copy() const { return { *this }; }