#ifndef MISC_H
#define MISC_H

#include <iostream>
#include <list>
using namespace std;

/// @brief Checks if a variable is an instance of <Base>
/// @tparam Base The class
/// @tparam T The type of the variable
/// @param ptr A pointer to the variable to test
/// @return `true` if `ptr` points to a variable that is an instance of `Base`
template<typename Base, typename T>
static inline bool instanceof(const T *ptr) {
  return dynamic_cast<const Base*>(ptr) != nullptr;
}

/// @brief Replaces all occurrences of a substring from a string. Note that `str` gets modified.
/// @param str The string
/// @param sub_str The substring to remove
/// @param replacement The replacement of the substring in the original string.
static inline void replace_substring(string& str, const string& sub_str, const string& replacement) {
  // "string::size_type" is `size_t` for strings
  string::size_type pos = str.find(sub_str);
  while (pos != string::npos) {
    str.replace(pos, sub_str.length(), replacement);
    pos = str.find(sub_str, pos + replacement.length());
  }
}

/// @brief Removes all occurrences of a pattern from a string.
/// @param str The string
/// @param pattern The pattern to remove.
static inline void remove_substring(string& str, const string& pattern) {
  string::size_type i = str.find(pattern);
  while (i != string::npos) {
    str.erase(i, pattern.length());
    i = str.find(pattern, i);
  }
}

/// @brief Removes all occurrences of a single character from a string.
/// @param str The string
/// @param character The character to remove
static inline void remove_character(string& str, const char& character) {
  string::size_type i = str.find(character);
  while (i != string::npos) {
    str.erase(i, 1);
    i = str.find(character, i);
  }
}

/// @brief Checks if a std::list contains an element.
/// @param l The list
/// @param element The element to look for
/// @return `true` if the list contains the element.
template<typename C, typename T>
static inline bool list_contains(C&& c, T e) { 
  return std::find(std::begin(c), std::end(c), e) != std::end(c);
}

/// @brief Checks if a string contains a substring.
/// @param str The main string.
/// @param substr The sub string to look for.
/// @return `true` if `substr` is contained within `str`
static inline bool string_contains(const string& str, const char substr) {
  return str.find(substr) != string::npos;
}

/// @brief Checks if two lists contain the same values. It uses the "==" operator on each value.
/// @tparam T The type of both lists, obviously the lists should be of the same type.
/// @param l1 The first list.
/// @param l2 The second list.
/// @return `true` if both lists contain the same values.
template<typename T>
static inline bool list_equals(const list<T>& l1, const list<T>& l2) {
  if (l1.size() != l2.size()) return false;
  // note that "typename" is necessary to make C++ understand
  // that the type `list<T>` refers to the template's type parameter
  typename list<T>::const_iterator iter1 = l1.begin(); // `const_iterator` is necessary because I'm using constant lists
  typename list<T>::const_iterator iter2 = l2.begin(); // indeed, `iterator` alone allows you the modify the list
  while (iter1 != l1.end()) {
    if (!(*iter1 == *iter2)) {
      return false;
    }
    ++iter1;
    ++iter2;
  }
  return true;
}

#endif