#pragma once

#include <list>
#include "../token.hpp"

/// @brief Gets the name of a token, useful for debugging.
/// @param type The type of the token (the value of the `TokenType` enum).
/// @return A string with the name of the value in the enum.
std::string get_token_name(TokenType type);

/// @brief Displays a list of tokens.
/// @param l The list that contains all the tokens.
/// @return A string to be displayed for debugging purposes.
std::string display_tokens_list(const std::list<std::unique_ptr<const Token>>& l);

/// @brief Displays the type of each token contained in the given list.
/// @param l The list of `TokenType`
/// @return A string that represents the list of all the tokens contained in the given list.
std::string display_tokens_list(const std::list<TokenType>& l);