#include "../../include/exceptions/expected_char_error.hpp"
using namespace std;

ExpectedCharError::ExpectedCharError(
  const Position& start,
  const Position& end,
  const string& d
): CustomError(start, end, "Expected Character", d) {}