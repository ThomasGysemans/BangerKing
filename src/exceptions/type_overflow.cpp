#include "../../include/exceptions/type_overflow_error.hpp"

TypeOverflowError::TypeOverflowError(
  const Position& start,
  const Position& end,
  const string& d
): CustomError(start, end, "Type Overflow", d) {}