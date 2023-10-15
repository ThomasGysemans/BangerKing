#include "../../include/exceptions/invalid_syntax_error.hpp"

InvalidSyntaxError::InvalidSyntaxError(
  const Position& start,
  const Position& end,
  const string& d
): CustomError(start, end, "Invalid Syntax", d) {}