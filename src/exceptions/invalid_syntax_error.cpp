#include "../../include/exceptions/invalid_syntax_error.hpp"
using namespace std;

InvalidSyntaxError::InvalidSyntaxError(
  const Position& start,
  const Position& end,
  const string& d
): CustomError(start, end, "Invalid Syntax", d) {}