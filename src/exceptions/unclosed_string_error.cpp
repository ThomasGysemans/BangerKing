#include "../../include/exceptions/unclosed_string_error.hpp"
using namespace std;

UnclosedStringError::UnclosedStringError(
  const Position& start,
  const Position& end,
  const string& d
): CustomError(start, end, "Unclosed string literal", d) {}