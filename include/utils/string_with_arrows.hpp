#pragma once

#include "../position.hpp"
#include "../miscellaneous.hpp"

/// @brief Draws a line of arrows below an error in the shell.
/// @param text The source code.
/// @param pos_start The starting position of the error.
/// @param pos_end The end position of the error.
/// @return The underlined error in the source code.
std::string string_with_arrows(const std::string& text, const Position& pos_start, const Position& pos_end);