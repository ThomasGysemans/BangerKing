#pragma once

#include <list>
#include "nodes/custom_node.hpp"
using namespace std;

// ? NOTE: templated functions MUST be defined in their header file, or a "undefined symbol" error will be triggered at compile time

/// @brief Checks if a variable is an instance of <Base>
/// @tparam Base The class
/// @tparam T The type of the variable
/// @param ptr A pointer to the variable to test
/// @return `true` if `ptr` points to a variable that is an instance of `Base`
template <typename Base, typename T>
bool instanceof(const T *ptr) {
  return dynamic_cast<const Base*>(ptr) != nullptr;
}

/// @brief Replaces all occurrences of a substring from a string. Note that `str` gets modified.
/// @param str The string
/// @param sub_str The substring to remove
/// @param replacement The replacement of the substring in the original string.
void replace_substring(string& str, const string& sub_str, const string& replacement);

/// @brief Removes all occurrences of a pattern from a string.
/// @param str The string
/// @param pattern The pattern to remove.
void remove_substring(string& str, const string& pattern);

/// @brief Removes all occurrences of a single character from a string.
/// @param str The string
/// @param character The character to remove
void remove_character(string& str, const char& character);

/// @brief Checks if a string contains a substring.
/// @param str The main string.
/// @param substr The sub string to look for.
/// @return `true` if `substr` is contained within `str`
bool string_contains(const string& str, const char substr);

/// @brief Casts an instance of CustomNode into a derived class of CustomNode. Note that it will throw an error if the cast isn't impossible.
/// @tparam T The expected type of the derived class.
/// @param b The instance of CustomNode.
/// @return The instance of the derived class `T` from `b`
template <typename T>
const T* cast_node(const CustomNode* b) {
  if (!instanceof<T>(b)) {
    throw string("incorrect cast of CustomNode");
  }
  return dynamic_cast<const T*>(b);
}