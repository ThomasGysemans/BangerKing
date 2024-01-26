#include "../../include/exceptions/unclosed_string_error.hpp"

UnclosedStringError::UnclosedStringError(
  const Position& start,
  const Position& end,
  const string& d
): CustomError(start, end, "Unclosed string literal", d) {}