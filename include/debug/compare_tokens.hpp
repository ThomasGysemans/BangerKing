#pragma once

#include <list>
#include "../token.hpp"
using namespace std;

/// @brief Compares a list of expected token's types and the list of tokens that the lexer returned.
/// @param expected The expected type of each token that the lexer should produce.
/// @param actual The actual list of tokens that the lexer returns.
/// @return `true` if both lists are equal.
bool compare_tokens(const list<TokenType>& expected, const list<unique_ptr<const Token>>& actual);