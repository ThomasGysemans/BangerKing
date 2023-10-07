#ifndef DEBUG_COMPARE_TOKENS_H
#define DEBUG_COMPARE_TOKENS_H

#include <iostream>
#include <list>
#include "../token.h"
using namespace std;

/// @brief Compares a list of expected token's types and the list of tokens that the lexer returned.
/// @param expected The expected type of each token that the lexer should produce.
/// @param actual The actual list of tokens that the lexer returns.
/// @return `true` if both lists are equal.
bool compare_tokens(const list<TokenType>& expected, const list<Token*>& actual) {
  list<TokenType> actual_tokens_list;
  list<Token*>::const_iterator iter = actual.begin();
  while (iter != actual.end()) {
    actual_tokens_list.push_back((*iter)->getType());
    ++iter;
  }
  return list_equals<TokenType>(expected, actual_tokens_list);
}

#endif