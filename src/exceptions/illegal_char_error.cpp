#include "../../include/exceptions/illegal_char_error.hpp"
using namespace std;

IllegalCharError::IllegalCharError(
  const Position& start,
  const Position& end,
  const string& d
): CustomError(start, end, "Illegal Character", d) {}