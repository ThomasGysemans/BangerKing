#pragma once

#include "exceptions/exception.hpp"
#include <string>

// Forward declarations are enough for the few functions using these
class CustomNode;
class Value;

// NOTE: templated functions MUST be defined in their header file, or a "undefined symbol" error will be triggered at compile time

/// @brief Checks if a variable is an instance of <Base>
/// @tparam Base The class
/// @tparam T The type of the variable
/// @param ptr A pointer to the variable to test
/// @return `true` if `ptr` points to a variable that is an instance of `Base`
template <typename Base, typename T>
bool instanceof(const T* ptr) {
  return dynamic_cast<const Base*>(ptr) != nullptr;
}

/// @brief Checks if a unique pointer is an instance of <Base>
/// @tparam Base The class
/// @tparam T The type of the variable
/// @param ptr A pointer to the variable to test
/// @return `true` if `ptr` points to a variable that is an instance of `Base`
template <typename Base, typename T>
bool instanceof(std::unique_ptr<T>& ptr) {
  return dynamic_cast<const Base*>(ptr.get()) != nullptr;
}

/// @brief Checks if a shared pointer is an instance of <Base>
/// @tparam Base The class
/// @tparam T The type of the variable
/// @param ptr A pointer to the variable to test
/// @return `true` if `ptr` points to a variable that is an instance of `Base`
template <typename Base, typename T>
bool instanceof(std::shared_ptr<T>& ptr) {
  return instanceof<Base>(ptr.get());
}

/// @brief Casts an instance of CustomNode into a derived class of CustomNode. Note that it will throw an error if the cast is impossible.
/// @tparam T The expected type of the derived class.
/// @param b The instance of CustomNode.
/// @return The instance of the derived class `T` from `b`
template <typename T>
std::unique_ptr<T> cast_node(std::unique_ptr<CustomNode>&& b) {
  if (auto cast = dynamic_cast<T*>(b.get())) {
    b.release(); // "b" has to be released, since a copy is made
    return std::unique_ptr<T>(cast);
  } else {
    throw Exception("Fatal", "incorrect cast of CustomNode");
  }
}

/// @brief Casts an instance of Value into a derived class of Value.
/// It will throw an error if the cast isn't possible.
/// The original unique pointer `ptr` will be released.
/// @tparam T The expected type of the derived class.
/// @param ptr The instance of Value as a unique pointer.
/// @return  The instance of the derived class `T` from `ptr` in a unique pointer.
template <typename T>
std::unique_ptr<T> cast_value(std::unique_ptr<Value>&& ptr) {
  if (auto cast = dynamic_cast<T*>(ptr.get())) {
    ptr.release();
    return std::unique_ptr<T>(cast);
  } else {
    throw Exception("Fatal", "incorrect cast of Value");
  }
}

/// @brief Casts a shared pointer of Value to a derived class.
/// @tparam T The derived class.
/// @param base The instance of Value in a shared pointer.
/// @return The cast from Value to `T`
template <typename T>
std::shared_ptr<T> cast_value(std::shared_ptr<Value>& base) {
  return std::dynamic_pointer_cast<T>(base);
}

/// @brief Casts a shared pointer of a constant instance of Value to a derived class.
/// @tparam T The derived class.
/// @param base The instance of Value in a shared pointer.
/// @return The cast from Value to `T`
template <typename T>
std::shared_ptr<const T> cast_const_value(std::shared_ptr<const Value>& base) {
  if (auto c = std::dynamic_pointer_cast<const T>(base)) {
    return c;
  } else {
    throw Exception("Fatal", "Invalid cast of value");
  }
}

/// @brief Replaces all occurrences of a substring from a string. Note that `str` gets modified.
/// @param str The string
/// @param sub_str The substring to remove
/// @param replacement The replacement of the substring in the original string.
void replace_substring(std::string& str, const std::string& sub_str, const std::string& replacement);

/// @brief Removes all occurrences of a pattern from a string.
/// @param str The string
/// @param pattern The pattern to remove.
void remove_substring(std::string& str, const std::string& pattern);

/// @brief Removes all occurrences of a single character from a string.
/// @param str The string
/// @param character The character to remove
void remove_character(std::string& str, const char& character);

/// @brief Checks if a string contains a particular character.
/// @param str The main string.
/// @param substr The character to look for.
/// @return `true` if character is contained within `str`
bool string_contains(const std::string& str, const char& substr);