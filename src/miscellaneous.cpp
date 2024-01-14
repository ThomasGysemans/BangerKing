#include "../include/miscellaneous.hpp"

void replace_substring(string& str, const string& sub_str, const string& replacement) {
  // "string::size_type" is `size_t` for strings
  string::size_type pos = str.find(sub_str);
  while (pos != string::npos) {
    str.replace(pos, sub_str.length(), replacement);
    pos = str.find(sub_str, pos + replacement.length());
  }
}

void remove_substring(string& str, const string& pattern) {
  string::size_type i = str.find(pattern);
  while (i != string::npos) {
    str.erase(i, pattern.length());
    i = str.find(pattern, i);
  }
}

void remove_character(string& str, const char& character) {
  string::size_type i = str.find(character);
  while (i != string::npos) {
    str.erase(i, 1);
    i = str.find(character, i);
  }
}

bool string_contains(const string& str, const char substr) {
  return str.find(substr) != string::npos;
}
