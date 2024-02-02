#pragma once

#include <list>
#include "../token.hpp"

/// @brief Compares a list of expected token's types and the list of tokens that the lexer returned.
/// @param expected The expected type of each token that the lexer should produce.
/// @param actual The actual list of tokens that the lexer returns.
/// @return `true` if both lists are equal.
bool compare_tokens(const std::list<TokenType>& expected, const std::list<std::shared_ptr<const Token>>& actual);