#pragma once

#include <iostream>
#include <string>
using namespace std;

/// @brief Prints a message with a custom color.
/// @param message The message to display (the title of a test for example).
void print_title(const string& message) {
  cout << "\e[0;32m" + message + "\e[0m" << endl;
}

/// @brief Returns a success message with a custom color using ANSI.
/// @param message The message to display.
/// @param indentation The level of indentation before the message.
/// @return The message in a custom color.
void print_success_msg(const string& message, int indentation = 0) {
  cout << "\e[0;32m" + std::string(indentation, '\t') + "✔ " + message + "\e[0m" << endl;
}