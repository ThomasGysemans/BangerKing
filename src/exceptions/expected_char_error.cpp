#include "../../include/exceptions/expected_char_error.hpp"

ExpectedCharError::ExpectedCharError(
  const Position& start,
  const Position& end,
  const string& d
): CustomError(start, end, "Expected Character", d) {}