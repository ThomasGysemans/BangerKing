#ifndef PRINT_TOKENS_H
#define PRINT_TOKENS_H

#include <iostream>
#include <list>
#include "../token.h"

using namespace std;

/// @brief Gets the name of a token, useful for debugging.
/// @param type The type of the token (the value of the `TokenType` enum).
/// @return A string with the name of the value in the enum.
string get_token_name(TokenType type) {
  switch (type) {
    case TokenType::NUMBER: return "NUMBER";
    case TokenType::PLUS: return "PLUS";
    case TokenType::MINUS: return "MINUS";
    case TokenType::MULTIPLY: return "MULTIPLY";
    case TokenType::SLASH: return "SLASH";
    case TokenType::POWER: return "POWER";
    case TokenType::MODULO: return "MODULO";
    case TokenType::LPAREN: return "LPAREN";
    case TokenType::RPAREN: return "RPAREN";
    case TokenType::IDENTIFIER: return "IDENTIFIER";
    case TokenType::KEYWORD: return "KEYWORD";
    case TokenType::EQUALS: return "EQUALS";
    case TokenType::DOUBLE_EQUALS: return "DOUBLE_EQUALS";
    case TokenType::NOT_EQUAL: return "NOT_EQUAL";
    case TokenType::LT: return "LT";
    case TokenType::GT: return "GT";
    case TokenType::LTE: return "LTE";
    case TokenType::GTE: return "GTE";
    case TokenType::COLON: return "COLON";
    case TokenType::NEWLINE: return "NEWLINE";
    case TokenType::LSQUARE: return "LSQUARE";
    case TokenType::RSQUARE: return "RSQUARE";
    case TokenType::LBRACK: return "LBRACK";
    case TokenType::RBRACK: return "RBRACK";
    case TokenType::COMMA: return "COMMA";
    case TokenType::STR: return "STRING";
    case TokenType::DOT: return "DOT";
    case TokenType::LCHEVRON: return "LCHEVRON";
    case TokenType::RCHEVRON: return "RCHEVRON";
    case TokenType::HASH: return "HASH";
    case TokenType::INC: return "INC";
    case TokenType::DEC: return "DEC";
    default:
      return "UNKNOWN_TOKEN(" + std::to_string(type) + ")";
  }
}

/// @brief Displays a list of tokens.
/// @param l The list that contains all the tokens.
/// @return A string to be displayed for debugging.
string display_tokens_list(const list<Token*>& l) {
  list<Token*>::const_iterator iter = l.begin();
  string result = "Tokens(" + std::to_string(l.size()) + ") : ";
  result += "[" + get_token_name((*iter)->getType()) + ":" + (*iter)->getStringValue();
  ++iter;
  while (iter != l.end()) {
    result += ", " + get_token_name((*iter)->getType()) + ":" + (*iter)->getStringValue();
    ++iter;
  }
  return result + "]";
}

/// @brief Displays the type of each token contained in the given list.
/// @param l The list of `TokenType`
/// @return A string that represents the list of all the tokens contained in the given list.
string display_tokens_list(const list<TokenType>& l) {
  list<TokenType>::const_iterator iter = l.begin();
  string result = "Tokens(" + std::to_string(l.size()) + ") : ";
  result += "[" + get_token_name(*iter);
  ++iter;
  while (iter != l.end()) {
    result += ", " + get_token_name(*iter);
    ++iter;
  }
  return result + "]";
}

#endif