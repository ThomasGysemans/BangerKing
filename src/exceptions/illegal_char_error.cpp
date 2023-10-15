#include "../../include/exceptions/illegal_char_error.hpp"

IllegalCharError::IllegalCharError(
  const Position& start,
  const Position& end,
  const string& d
): CustomError(start, end, "Illegal Character", d) {}