#ifndef TESTS_HELPER_H
#define TESTS_HELPER_H

#include <iostream>
using namespace std;

/// @brief Returns a success message with a blueish color using ANSI.
/// @param message The message to display.
/// @param indentation The level of indentation before the message.
/// @return The message in a blueish color.
static inline string get_success_msg(const string& message, int indentation = 0) {
  return "\e[0;32m" + std::string(indentation, '\t') + "✔ " + message + "\e[0m";
}

#endif